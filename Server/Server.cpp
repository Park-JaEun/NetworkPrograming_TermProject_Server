#include "pch.h"
#include "CTimer.h"
#include "CObjectMgr.h"
#include "CCollisionMgr.h"
#include "CCollider.h"
#include "CEventMgr.h"
#include "CBullet.h"

#include <thread>

std::mutex g_mutex;
int clientId = 0;		// Å¬¶óÀÌ¾ğÆ® id ÃÊ±â°ª 0
int bulletId = 0;		// Åõ»çÃ¼ id ÃÊ±â°ª 0

std::vector<PlayerInfo> ClientInfo;					// Å¬¶óÀÌ¾ğÆ® Á¤º¸ º¤ÅÍ

// ÇÔ¼ö¸¦ »ç¿ëÇÏ¿© Å¬¶óÀÌ¾ğÆ® Ã³¸®
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;	// Å¬¶óÀÌ¾ğÆ® ¼ÒÄÏ
	struct sockaddr_in clientaddr;		// Å¬¶óÀÌ¾ğÆ® ÁÖ¼Ò
	char nick_name[256];				// Å¬¶óÀÌ¾ğÆ® ´Ğ³×ÀÓ
	char buf[BUFSIZE];					// ÆĞÅ¶ µ¥ÀÌÅÍ ¹öÆÛ
	int retval;							// ¸®ÅÏ°ª
	int addrlen;						// ÁÖ¼Ò ±æÀÌ
	int size;							// ÆĞÅ¶ »çÀÌÁî
	bool isAllReady{ false };			// ¸ğµç Å¬¶óÀÌ¾ğÆ®°¡ ÁØºñ »óÅÂÀÎÁö È®ÀÎ		(Ä³¸¯ÅÍ ¼±ÅÃ ¿Ï·á)
	bool isAllInit{ false };			// ¸ğµç Å¬¶óÀÌ¾ğÆ®°¡ ÃÊ±âÈ­ »óÅÂÀÎÁö È®ÀÎ	(½ºÅ×ÀÌÁö ÃÊ±âÈ­ ¿Ï·á)
	bool isBoss{ false };				// º¸½º »ı¼º ¿©ºÎ
	bool isGameOver{ false };			// °ÔÀÓ ¿À¹ö ¿©ºÎ
	bool IsGameClear{ false };			// °ÔÀÓ Å¬¸®¾î ¿©ºÎ
	CObject* pCharacter = new CPlayer;

	// Á¢¼ÓÇÑ Å¬¶óÀÌ¾ğÆ® Á¤º¸ ÀúÀå
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// Å¬¶óÀÌ¾ğÆ®ÀÇ ´Ğ³×ÀÓ ¼ö½Å
	retval = recv(client_sock, (char*)&size, sizeof(int), MSG_WAITALL);
	retval = recv(client_sock, nick_name, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv(): Client Nickname");
		closesocket(client_sock);
		return 0;
	}
	else if (retval == 0)
		return 0;
	nick_name[retval] = '\0';

	// PlayerInfo ÀÎ½ºÅÏ½º¸¦ »ı¼ºÇÏ¿© ClientInfo º¤ÅÍ¿¡ Ãß°¡
	PlayerInfo player;
	{
		std::lock_guard<std::mutex> lock{ g_mutex };		// ¹ÂÅØ½º Àá±İ
		player.nickname = nick_name;						// Å¬¶óÀÌ¾ğÆ® ´Ğ³×ÀÓ ÀúÀå
		player.portnumber = ntohs(clientaddr.sin_port);		// ³×Æ®¿öÅ© ¹ÙÀÌÆ® ¼ø¼­¿¡¼­ È£½ºÆ® ¹ÙÀÌÆ® ¼ø¼­·Î º¯È¯
		player.id = clientId++;								// Å¬¶óÀÌ¾ğÆ® ID ÇÒ´ç
		player.isReady = false;								// Å¬¶óÀÌ¾ğÆ® ÁØºñ »óÅÂ false·Î ÃÊ±âÈ­
		player.isInit = false;								// Å¬¶óÀÌ¾ğÆ® ÃÊ±âÈ­ »óÅÂ false·Î ÃÊ±âÈ­

		// Å¬¶óÀÌ¾ğÆ®ÀÇ Á¤º¸¸¦ º¤ÅÍ¿¡ ÀúÀå(´Ğ³×ÀÓ, Æ÷Æ®¹øÈ£, ¾ÆÀÌµğ)
		ClientInfo.emplace_back(player);

		// Á¢¼ÓÇÑ Å¬¶óÀÌ¾ğÆ® Á¤º¸¿Í ´Ğ³×ÀÓ Ãâ·Â
		std::cout << "\nPlayer " << player.id << " Á¢¼Ó: IP ÁÖ¼Ò = " << addr << ", Æ÷Æ® ¹øÈ£ = " << ntohs(clientaddr.sin_port) << ", ´Ğ³×ÀÓ = " << nick_name << std::endl;
	}

	// Å¬¶óÀÌ¾ğÆ® Á¤º¸ ¼Û½Å
	SC_MAKE_ID_PACKET idPacket;
	idPacket.type	= static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
	idPacket.id		= player.id;
	size = sizeof(SC_MAKE_ID_PACKET);

	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(client_sock, reinterpret_cast<char*>(&idPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}

	std::cout << "[Player" << idPacket.id << "] ID ÇÒ´ç ÆĞÅ¶ Àü¼Û" << std::endl;

	// Å¬¶óÀÌ¾ğÆ® Ä³¸¯ÅÍ ¼±ÅÃ Á¤º¸ ¼ö½Å
	retval = recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
	retval = recv(client_sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return 0;
	}
	else if (retval == 0)
		return 0;

	SELECT_CHARACTER_PACKET* pSelectPacket = reinterpret_cast<SELECT_CHARACTER_PACKET*>(buf);
	{
		std::lock_guard<std::mutex> lock{ g_mutex };
		((CPlayer*)pCharacter)->SetType(pSelectPacket->character);
		((CPlayer*)pCharacter)->SetName(L"Player" + std::to_wstring(player.id));
		((CPlayer*)pCharacter)->SetPos(Vec2(5000.f, 0.f));
		((CPlayer*)pCharacter)->SetDir(DIR_RIGHT);
		((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
		((CPlayer*)pCharacter)->CreateCollider();
		((CPlayer*)pCharacter)->GetCollider()->SetScale(Vec2(31.f, 30.f));

		ClientInfo[player.id].isReady = true;	// Å¬¶óÀÌ¾ğÆ® ÁØºñ »óÅÂ true·Î º¯°æ
		CObjectMgr::GetInst()->AddObject(pCharacter, GROUP_TYPE::PLAYER);	// ÇÃ·¹ÀÌ¾î °´Ã¼ µî·Ï
	}

	switch (pSelectPacket->character) {
	case CHARACTER_TYPE::MINJI:
		std::cout << "[Player" << player.id << "] " << "¹ÎÁö ¼±ÅÃ" << std::endl;
		break;

	case CHARACTER_TYPE::HANNIE:
		std::cout << "[Player" << player.id << "] " << "ÇÏ´Ï ¼±ÅÃ" << std::endl;
		break;

	case CHARACTER_TYPE::DANIELLE:
		std::cout << "[Player" << player.id << "] " << "´Ù´Ï¿¤ ¼±ÅÃ" << std::endl;
		break;

	case CHARACTER_TYPE::HEARIN:
		std::cout << "[Player" << player.id << "] " << "Çı¸° ¼±ÅÃ" << std::endl;
		break;

	case CHARACTER_TYPE::HYEIN:
		std::cout << "[Player" << player.id << "] " << "ÇıÀÎ ¼±ÅÃ" << std::endl;
		break;

	default:
		break;
	}

	// Å¬¶óÀÌ¾ğÆ®°¡ 3¸í µé¾î¿ÍÀÖ°í, ¸ğµÎ ÁØºñ »óÅÂÀÏ °æ¿ì ÇÃ·¹ÀÌ¾îµé¿¡°Ô ÃÊ±âÈ­ ½ÅÈ£ ÆĞÅ¶ Àü¼Û
	while (1) {
		std::lock_guard<std::mutex> lock{ g_mutex };	// ¹ÂÅØ½º Àá±İ
		isAllReady = true;

		for (const PlayerInfo& info : ClientInfo) {
			if (!info.isReady) {
				isAllReady = false;
				break;
			}
		}

		if (isAllReady && ClientInfo.size() == MAX_PLAYER)	// ¸ğµç Å¬¶óÀÌ¾ğÆ®°¡ ÁØºñ »óÅÂÀÌ¸é ·çÇÁ Å»Ãâ
			break;
	}

	SC_INIT_PACKET initPacket;
	size = sizeof(SC_INIT_PACKET);
	initPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_INIT);

	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(client_sock, reinterpret_cast<char*>(&initPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}

	std::cout << "¸ğµç ÇÃ·¹ÀÌ¾î ÁØºñ ¿Ï·á" << std::endl;
	std::cout << "ÇÃ·¹ÀÌ¾îµé¿¡°Ô ÃÊ±âÈ­ ½ÅÈ£ ÆĞÅ¶ Àü¼Û" << std::endl;

	// º»ÀÎÀ» Á¦¿ÜÇÑ ´Ù¸¥ Å¬¶óÀÌ¾ğÆ®µéÀÇ ÇÃ·¹ÀÌ¾î Á¤º¸ ¼Û½Å
	for (const PlayerInfo& otherPlayerInfo : ClientInfo) {
		if (otherPlayerInfo.id != player.id) {
			CObject* pOtherPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(otherPlayerInfo.id));

			SC_PLAYER_PACKET otherPlayerPacket;
			otherPlayerPacket.type			= static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
			otherPlayerPacket.playerID		= otherPlayerInfo.id;
			otherPlayerPacket.playerPos		= pOtherPlayer->GetPos();
			otherPlayerPacket.playerState	= ((CPlayer*)pOtherPlayer)->GetState();
			otherPlayerPacket.playerDir		= ((CPlayer*)pOtherPlayer)->GetDir();
			otherPlayerPacket.character		= ((CPlayer*)pOtherPlayer)->GetType();
			size = sizeof(otherPlayerPacket);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(client_sock, reinterpret_cast<char*>(&otherPlayerPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				return 0;
			}
		}
	}

	// ÃÊ±âÈ­ ¿Ï·á ½ÅÈ£ ÆĞÅ¶ ¼ö½Å
	retval = recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
	retval = recv(client_sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return 0;
	}
	else if (retval == 0)
		return 0;

	CS_INIT_FINISH_PACKET* initFinishPacket = reinterpret_cast<CS_INIT_FINISH_PACKET*>(buf);
	{
		std::lock_guard<std::mutex> lock{ g_mutex };	// ¹ÂÅØ½º Àá±İ
		ClientInfo[player.id].isInit = true;			// Å¬¶óÀÌ¾ğÆ® ÃÊ±âÈ­ »óÅÂ true·Î º¯°æ
		std::cout << "[Player" << player.id << "] ½ºÅ×ÀÌÁö ÃÊ±âÈ­ ¿Ï·á" << std::endl;
	}

	// ¸ğµç Å¬¶óÀÌ¾ğÆ®°¡ ÃÊ±âÈ­¸¦ ¿Ï·áÇÏ¸é ÇÃ·¹ÀÌ¾îµé¿¡°Ô °ÔÀÓ ½ÃÀÛ ½ÅÈ£ ÆĞÅ¶ Àü¼Û
	while (1) {
		std::lock_guard<std::mutex> lock{ g_mutex };
		isAllInit = true;

		for (const PlayerInfo& info : ClientInfo) {
			if (!info.isInit) {
				isAllInit = false;
				break;
			}
		}

		if (isAllInit && ClientInfo.size() == MAX_PLAYER)
			break;
	}

	SC_GAME_START_PACKET startPacket;
	startPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_START);
	size = sizeof(SC_GAME_START_PACKET);

	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(client_sock, reinterpret_cast<char*>(&startPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}

	std::cout << "¸ğµç ÇÃ·¹ÀÌ¾î ÃÊ±âÈ­ ¿Ï·á" << std::endl;
	std::cout << "ÇÃ·¹ÀÌ¾îµé¿¡°Ô °ÔÀÓ ½ÃÀÛ ½ÅÈ£ ÆĞÅ¶ Àü¼Û" << std::endl;

	while (1) {
		//Sleep(100 / 60);	// 600fps

		// - std::this_thread::sleep_for
		// : ÇöÀç ½º·¹µå¸¦ ÁöÁ¤µÈ ½Ã°£ µ¿¾È ÀáÀç¿ò
		// : Sleep°ú ´Ù¸¥ Á¡Àº	SleepÀº ÇöÀç ½º·¹µå¸¦ ÀáÀç¿ò, 
		// : std::this_thread::sleep_forÀº ÇöÀç ½º·¹µå¸¦ ÀáÀç¿ì°í ´Ù¸¥ ½º·¹µå¸¦ ½ÇÇà½ÃÅ´
		std::this_thread::sleep_for(std::chrono::milliseconds(100 / 60));
		// ¹öÆÛ ºñ¿ì±â
		memset(buf, 0, BUFSIZE);

		////////////
		// recv() //
		////////////

		// µ¥ÀÌÅÍ ¹Ş±â
		retval = recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
		retval = recv(client_sock, buf, size, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			//¹Ş¾Æ¿Â ÆĞÅ¶À» Å°º¸µå ÀÔ·Â ÆĞÅ¶À¸·Î Ä³½ºÆÃ
			CS_KEYBOARD_INPUT_PACKET* pKeyInputPacket = reinterpret_cast<CS_KEYBOARD_INPUT_PACKET*>(buf);

			// ÇÃ·¹ÀÌ¾î Ã¼·ÂÀÌ 0º¸´Ù Å©°í, °ÔÀÓ ¿À¹öµÇÁö ¾Ê¾ÒÀ» ¶§
			if (((CPlayer*)pCharacter)->GetHP() > 0 && !((CPlayer*)pCharacter)->GetIsGameOver()) {
				for (int i = 0; i < pKeyInputPacket->keyCount; ++i) {
					// NONEÀÌ ¾Æ´Ò ¶§¸¸	Ã³¸®
					if (pKeyInputPacket->inputs[i].key != KEY::LAST && pKeyInputPacket->inputs[i].key_state != KEY_STATE::NONE) {
						// ÇÃ·¹ÀÌ¾î À§Ä¡ ÀÚ±â°Í¸¸ ¼öÁ¤
						// °¢ ÇÃ·¹ÀÌ¾îÀÇ À§Ä¡ ¾÷µ¥ÀÌÆ®´Â °¢ Å¬¶óÀÌ¾ğÆ®ÀÇ ½º·¹µå¿¡¼­ ¹è¿­¿¡ µ¿½ÃÁ¢±ÙÇØ¼­ °¢ÀÚ ¹Ù²Ş
						Vec2 vCurPos = pCharacter->GetPos();
						Vec2 vDummyPos{};
						float speed = ((CPlayer*)pCharacter)->GetSpeed();

						// »ó
						if (pKeyInputPacket->inputs[i].key == KEY::UP && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
							vDummyPos = Vec2(vCurPos.x, vCurPos.y - speed * DT);

							if (IsInWorld(vDummyPos) && !isBoss)
								vCurPos.y -= speed * DT;
							else if (IsInBossRoom(vDummyPos) && isBoss)
								vCurPos.y -= speed * DT;

							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);
						}
						if (pKeyInputPacket->inputs[i].key == KEY::UP && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
						}

						// ÇÏ
						if (pKeyInputPacket->inputs[i].key == KEY::DOWN && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
							vDummyPos = Vec2(vCurPos.x, vCurPos.y + speed * DT);

							if (IsInWorld(vDummyPos) && !isBoss)
								vCurPos.y += speed * DT;
							else if (IsInBossRoom(vDummyPos) && isBoss)
								vCurPos.y += speed * DT;

							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);
						}
						if (pKeyInputPacket->inputs[i].key == KEY::DOWN && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
						}

						// ÁÂ
						if (pKeyInputPacket->inputs[i].key == KEY::LEFT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
							vDummyPos = Vec2(vCurPos.x - speed * DT, vCurPos.y);

							if (IsInWorld(vDummyPos) && !isBoss)
								vCurPos.x -= speed * DT;
							else if (IsInBossRoom(vDummyPos) && isBoss)
								vCurPos.x -= speed * DT;

							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);

							if (((CPlayer*)pCharacter)->GetDir() != DIR_LEFT)
								((CPlayer*)pCharacter)->SetDir(DIR_LEFT);
						}
						if (pKeyInputPacket->inputs[i].key == KEY::LEFT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
						}

						// ¿ì
						if (pKeyInputPacket->inputs[i].key == KEY::RIGHT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
							vDummyPos = Vec2(vCurPos.x + speed * DT, vCurPos.y);

							if (IsInWorld(vDummyPos) && !isBoss)
								vCurPos.x += speed * DT;
							else if (IsInBossRoom(vDummyPos) && isBoss)
								vCurPos.x += speed * DT;

							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);

							if (((CPlayer*)pCharacter)->GetDir() != DIR_RIGHT)
								((CPlayer*)pCharacter)->SetDir(DIR_RIGHT);
						}
						if (pKeyInputPacket->inputs[i].key == KEY::RIGHT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
						}

						if (pKeyInputPacket->inputs[i].key == KEY::SPACE && pKeyInputPacket->inputs[i].key_state == KEY_STATE::TAP) {
<<<<<<< HEAD
							float timeSinceLastFire = std::chrono::duration<float>(now - lastFireTime).count();
							if (timeSinceLastFire >= fireRate) {
								// ÃÑ¾Ë ¹ß»ç
((CPlayer*)pCharacter)->CreateBullet(player.id, bulletId++);
lastFireTime = now; // ¸¶Áö¸· ¹ß»ç ½Ã°£ ¾÷µ¥ÀÌÆ®
							}
=======
							// ÃÑ¾Ë ¹ß»ç
							((CPlayer*)pCharacter)->CreateBullet(player.id, bulletId++);
>>>>>>> parent of 68322ae ([ê³µí†µ] í”Œë ˆì´ì–´ ìœ„ì¹˜ ì˜ˆì¸¡ê³¼ ë³´ê°„ ì¶”ê°€, ëª¨ë“  í´ë¼ì´ì–¸íŠ¸ í‚¤ì…ë ¥ ë™ê¸°í™” êµ¬í˜„)
						}

						// Ã³¸®ÇÑ Á¤º¸ ¾÷µ¥ÀÌÆ®
						((CPlayer*)pCharacter)->SetPos(vCurPos);
					}
				}
			}
			else if (((CPlayer*)pCharacter)->GetHP() <= 0) {
				if (((CPlayer*)pCharacter)->GetState() != PLAYER_STATE::DIE)
					((CPlayer*)pCharacter)->SetState(PLAYER_STATE::DIE);

				// 1.5ÃÊµÚ ¾Æ·¡·Î Ãß¶ô
				if (((CPlayer*)pCharacter)->GetDieTime() >= 1.5f && ((CPlayer*)pCharacter)->GetDieTime() <= 4.5)
					((CPlayer*)pCharacter)->SetPos(Vec2(((CPlayer*)pCharacter)->GetPos().x, ((CPlayer*)pCharacter)->GetPos().y + DT * 50.f * 3));
				else
					((CPlayer*)pCharacter)->SetDieTime(((CPlayer*)pCharacter)->GetDieTime() + DT);

				// ¾Æ·¡·Î Ãß¶ôÇßÀ¸¸é 3ÃÊÈÄ ºÎÈ°
				if (!((CPlayer*)pCharacter)->GetIsGameOver() && ((CPlayer*)pCharacter)->GetLife() >= 1 && ((CPlayer*)pCharacter)->GetResurrectTime() >= 3.0f && ((CPlayer*)pCharacter)->GetDieTime() >= 1.5f) {
					// Life °¨¼Ò
					((CPlayer*)pCharacter)->SetLife(((CPlayer*)pCharacter)->GetLife() - 1);

					// Life°¡ 0ÀÌ¸é °ÔÀÓ¿À¹ö
					if (((CPlayer*)pCharacter)->GetLife() == 0) {
						((CPlayer*)pCharacter)->SetIsGameOver(true);
					}
					else {
						// ÇÃ·¹ÀÌ¾î ÃÊ±âÈ­
						((CPlayer*)pCharacter)->SetHP(3);
						((CPlayer*)pCharacter)->SetDieTime(0.f);
						((CPlayer*)pCharacter)->SetResurrectTime(0.f);
						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
						((CPlayer*)pCharacter)->SetPos(Vec2(pCharacter->GetPos().x, 0.f));
					}
				}
				else if (((CPlayer*)pCharacter)->GetDieTime() >= 1.5f) {
					((CPlayer*)pCharacter)->SetResurrectTime(((CPlayer*)pCharacter)->GetResurrectTime() + DT);
				}
			}

			// ÇÃ·¹ÀÌ¾î Á¤º¸ ¼Û½Å
			SC_PLAYER_PACKET playerPacket;
			playerPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
			playerPacket.playerID = player.id;
			playerPacket.playerPos = pCharacter->GetPos();
			playerPacket.playerState = ((CPlayer*)pCharacter)->GetState();
			playerPacket.playerDir = ((CPlayer*)pCharacter)->GetDir();
			playerPacket.playerHP = ((CPlayer*)pCharacter)->GetHP();
			playerPacket.playerLife = ((CPlayer*)pCharacter)->GetLife();
			playerPacket.playerBunnyCount = ((CPlayer*)pCharacter)->GetBunnyCount();
			playerPacket.playerCookieCount = ((CPlayer*)pCharacter)->GetCookieCount();
			playerPacket.playerKillCount = ((CPlayer*)pCharacter)->GetKillCount();
			playerPacket.character = ((CPlayer*)pCharacter)->GetType();
			size = sizeof(playerPacket);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(client_sock, reinterpret_cast<char*>(&playerPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			// º»ÀÎÀ» Á¦¿ÜÇÑ ´Ù¸¥ Å¬¶óÀÌ¾ğÆ® Á¤º¸ ¼Û½Å
			for (const PlayerInfo& otherPlayerInfo : ClientInfo) {
				if (otherPlayerInfo.id != player.id) {
					CObject* pOtherPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(otherPlayerInfo.id));

					SC_PLAYER_PACKET otherPlayerPacket;
					otherPlayerPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
					otherPlayerPacket.playerID = otherPlayerInfo.id;
					otherPlayerPacket.playerPos = pOtherPlayer->GetPos();
					otherPlayerPacket.playerState = ((CPlayer*)pOtherPlayer)->GetState();
					otherPlayerPacket.playerDir = ((CPlayer*)pOtherPlayer)->GetDir();
					otherPlayerPacket.playerHP = ((CPlayer*)pOtherPlayer)->GetHP();
					otherPlayerPacket.playerLife = ((CPlayer*)pOtherPlayer)->GetLife();
					otherPlayerPacket.playerBunnyCount = ((CPlayer*)pOtherPlayer)->GetBunnyCount();
					otherPlayerPacket.playerCookieCount = ((CPlayer*)pOtherPlayer)->GetCookieCount();
					otherPlayerPacket.playerKillCount = ((CPlayer*)pOtherPlayer)->GetKillCount();
					otherPlayerPacket.character = ((CPlayer*)pOtherPlayer)->GetType();
					size = sizeof(otherPlayerPacket);

					retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
					retval = send(client_sock, reinterpret_cast<char*>(&otherPlayerPacket), size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send()");
						return 0;
					}
				}
			}
		}

		////////////
		// send() //
		////////////

		// ¸ğµç ÇÃ·¹ÀÌ¾îÀÇ Life°¡ 0ÀÌ¸é °ÔÀÓ ¿À¹ö
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecPlayer = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::PLAYER);

			int deadPlayerCount = 0;
			for (CObject* pPlayer : vecPlayer) {
				if (((CPlayer*)pPlayer)->GetLife() == 0)
					++deadPlayerCount;
			}

			if (deadPlayerCount == MAX_PLAYER) {
				isGameOver = true;
				IsGameClear = false;
			}
		}

		// ¸ó½ºÅÍ Á¤º¸ ¼Û½Å
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecMonster = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::MONSTER);

			SC_MONSTER_PACKET monsterPacket;
			monsterPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_MONSTER);
			size = sizeof(SC_MONSTER_PACKET);

			// ¸ó½ºÅÍ ¼ö Àü¼Û
			int monsterCount = vecMonster.size();
			retval = send(client_sock, reinterpret_cast<char*>(&monsterCount), sizeof(monsterCount), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			// ¸ó½ºÅÍ Á¤º¸ º¸³»±â
			for (CObject* pMonster : vecMonster) {
				monsterPacket.monsterID		= ((CMonster*)pMonster)->GetID();
				monsterPacket.monsterPos	= pMonster->GetPos();
				monsterPacket.monsterState	= ((CMonster*)pMonster)->GetState();
				monsterPacket.monsterDir	= ((CMonster*)pMonster)->GetDir();
				monsterPacket.monsterIsDead = pMonster->IsDead();

				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				retval = send(client_sock, reinterpret_cast<char*>(&monsterPacket), size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
			}
		}

		// º¸½º Á¤º¸ ¼Û½Å, º¸½º Á¤º¸´Â ¸ğµç ÇÃ·¹ÀÌ¾îÀÇ xÁÂÇ¥ À§Ä¡°¡ 5060.f ÀÌ»óÀÌ¸é º¸½º »ı¼º
		{
			// ¸ğµç ÇÃ·¹ÀÌ¾îÀÇ xÁÂÇ¥ À§Ä¡°¡ 5060.f ÀÌ»óÀÌ¸é isBoss¸¦ true·Î º¯°æ
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecBoss = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::BOSS);
			CObject* pBoss = CObjectMgr::GetInst()->FindObject(L"Boss");

			if (!isBoss) {
				for (int i = 0; i < MAX_PLAYER; ++i) {
					CObject* pPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(i));

					if (pPlayer->GetPos().x < 5060.f) {
						isBoss = false;
						break;
					}
					else
						isBoss = true;
				}

				if (isBoss) {
					// º¸½º ÃâÇö
					((CBoss*)pBoss)->SetHaveToAppear(true);
					((CBoss*)pBoss)->SetState(BOSS_STATE::IDLE);
					std::cout << "º¸½º ÃâÇö" << std::endl;
				}
				else {
					// º¸½º ¼ö º¸³»±â
					int bossCount = vecBoss.size();
					retval = send(client_sock, reinterpret_cast<char*>(&bossCount), sizeof(bossCount), 0);
					if (retval == SOCKET_ERROR) {
						err_display("send()");
						break;
					}

					// º¸½º Á¤º¸ º¸³»±â
					SC_BOSS_PACKET bossPacket;
					bossPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_BOSS);
					size = sizeof(SC_BOSS_PACKET);

					bossPacket.bossPos = Vec2(0.f, 0.f);
					bossPacket.bossState = BOSS_STATE::NOT_APPEAR;
					bossPacket.bossIsDead = false;

					retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
					retval = send(client_sock, reinterpret_cast<char*>(&bossPacket), size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send()");
						break;
					}
				}
			}

			// º¸½º Á¤º¸ ¼Û½Å (À§Ä¡, »óÅÂ)
			if (isBoss) {
				// º¸½º ¼ö º¸³»±â
				int bossCount = vecBoss.size();
				retval = send(client_sock, reinterpret_cast<char*>(&bossCount), sizeof(bossCount), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}

				// º¸½º Á¤º¸ º¸³»±â
				SC_BOSS_PACKET bossPacket;
				bossPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_BOSS);
				size = sizeof(SC_BOSS_PACKET);

				for (CObject* pBoss : vecBoss) {
					bossPacket.bossPos		= pBoss->GetPos();
					bossPacket.bossState	= ((CBoss*)pBoss)->GetState();
					bossPacket.bossIsDead	= pBoss->IsDead();
					
					// º¸½º°¡ Á×¾úÀ¸¸é °ÔÀÓ Å¬¸®¾î
					if (pBoss->IsDead()) {
						IsGameClear = true;
					}

					retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
					retval = send(client_sock, reinterpret_cast<char*>(&bossPacket), size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send()");
						break;
					}
				}
			}
		}

		// Åä³¢ ¾ÆÀÌÅÛ Á¤º¸ ¼Û½Å
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecRabbitItem = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::ITEM_RABBIT);

			SC_RABBIT_ITEM_PACKET rabbitItemPacket;
			rabbitItemPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_RABBIT_ITEM);
			size = sizeof(SC_RABBIT_ITEM_PACKET);

			// Åä³¢ ¾ÆÀÌÅÛ ¼ö Àü¼Û
			int rabbitItemCount = vecRabbitItem.size();
			retval = send(client_sock, reinterpret_cast<char*>(&rabbitItemCount), sizeof(rabbitItemCount), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			// Åä³¢ ¾ÆÀÌÅÛ Á¤º¸ º¸³»±â
			for (CObject* pRabbitItem : vecRabbitItem) {
				rabbitItemPacket.itemID		= ((CItem*)pRabbitItem)->GetID();
				rabbitItemPacket.itemPos	= ((CItem*)pRabbitItem)->GetPos();
				rabbitItemPacket.itemIsDead = pRabbitItem->IsDead();

				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				retval = send(client_sock, reinterpret_cast<char*>(&rabbitItemPacket), size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
			}
		}

		// ÄíÅ° ¾ÆÀÌÅÛ Á¤º¸ ¼Û½Å
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecCookieItem = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::ITEM_COOKIE);

			SC_COOKIE_ITEM_PACKET cookieItemPacket;
			cookieItemPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_COOKIE_ITEM);
			size = sizeof(SC_COOKIE_ITEM_PACKET);

			// ÄíÅ° ¾ÆÀÌÅÛ ¼ö Àü¼Û
			int cookieItemCount = vecCookieItem.size();
			retval = send(client_sock, reinterpret_cast<char*>(&cookieItemCount), sizeof(cookieItemCount), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			// ÄíÅ° ¾ÆÀÌÅÛ Á¤º¸ º¸³»±â
			for (CObject* pCookieItem : vecCookieItem) {
				cookieItemPacket.itemID		= ((CItem*)pCookieItem)->GetID();
				cookieItemPacket.itemPos	= ((CItem*)pCookieItem)->GetPos();
				cookieItemPacket.itemIsDead = pCookieItem->IsDead();

				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				retval = send(client_sock, reinterpret_cast<char*>(&cookieItemPacket), size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
			}
		}

		// ÇÃ·¹ÀÌ¾î Åõ»çÃ¼ Á¤º¸ ¼Û½Å
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecBullet = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::BULLET_PLAYER);

			SC_BULLET_PACKET bulletPacket;
			bulletPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
			size = sizeof(SC_BULLET_PACKET);

			// Å¬¶óÀÌ¾ğÆ®¿¡°Ô ÇÃ·¹ÀÌ¾î Åõ»çÃ¼ ¼ö Àü¼Û
			int bulletCount = vecBullet.size();
			retval = send(client_sock, reinterpret_cast<char*>(&bulletCount), sizeof(bulletCount), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			if (bulletCount != 0) {
				// Å¬¶óÀÌ¾ğÆ®¿¡ ÇÃ·¹ÀÌ¾î Åõ»çÃ¼ Á¤º¸ º¸³»±â
				for (CObject* pBullet : vecBullet) {
					bulletPacket.playerID		= ((CBullet*)pBullet)->GetPlayerID();	// ¹ß»çÇÑ ÇÃ·¹ÀÌ¾î id
					bulletPacket.bulletID		= ((CBullet*)pBullet)->GetID();
					bulletPacket.bulletPos		= ((CBullet*)pBullet)->GetPos();
					bulletPacket.bulletIsDead	= ((CBullet*)pBullet)->IsDead();
					bulletPacket.bulletDir		= ((CBullet*)pBullet)->GetDir();
					bulletPacket.bulletDegree	= 0.f;

					retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
					retval = send(client_sock, reinterpret_cast<char*>(&bulletPacket), size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send()");
						break;
					}
				}
			}
		}

		// ¸ó½ºÅÍ Åõ»çÃ¼ Á¤º¸ ¼Û½Å
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecBullet = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::BULLET_MONSTER);

			SC_BULLET_PACKET bulletPacket;
			bulletPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
			size = sizeof(SC_BULLET_PACKET);

			// Å¬¶óÀÌ¾ğÆ®¿¡°Ô ¸ó½ºÅÍ Åõ»çÃ¼ ¼ö Àü¼Û
			int bulletCount = vecBullet.size();
			retval = send(client_sock, reinterpret_cast<char*>(&bulletCount), sizeof(bulletCount), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			if (bulletCount != 0) {
				// Å¬¶óÀÌ¾ğÆ®¿¡ ¸ó½ºÅÍ Åõ»çÃ¼ Á¤º¸ º¸³»±â
				for (CObject* pBullet : vecBullet) {
					bulletPacket.playerID		= ((CBullet*)pBullet)->GetPlayerID();	// ¹ß»çÇÑ ÇÃ·¹ÀÌ¾î id
					bulletPacket.bulletID		= ((CBullet*)pBullet)->GetID();
					bulletPacket.bulletPos		= ((CBullet*)pBullet)->GetPos();
					bulletPacket.bulletIsDead	= ((CBullet*)pBullet)->IsDead();
					bulletPacket.bulletDir		= ((CBullet*)pBullet)->GetDir();
					bulletPacket.bulletDegree	= 0.f;

					retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
					retval = send(client_sock, reinterpret_cast<char*>(&bulletPacket), size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send()");
						break;
					}
				}
			}
		}

		// º¸½º Åõ»çÃ¼ Á¤º¸ ¼Û½Å
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecBullet = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::BULLET_BOSS);

			SC_BULLET_PACKET bulletPacket;
			bulletPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
			size = sizeof(SC_BULLET_PACKET);

			// Å¬¶óÀÌ¾ğÆ®¿¡°Ô º¸½º Åõ»çÃ¼ ¼ö Àü¼Û
			int bulletCount = vecBullet.size();
			retval = send(client_sock, reinterpret_cast<char*>(&bulletCount), sizeof(bulletCount), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			if (bulletCount != 0) {
				// Å¬¶óÀÌ¾ğÆ®¿¡°Ô º¸½º Åõ»çÃ¼ Á¤º¸ º¸³»±â
				for (CObject* pBullet : vecBullet) {
					bulletPacket.playerID		= ((CBullet*)pBullet)->GetPlayerID();
					bulletPacket.bulletID		= ((CBullet*)pBullet)->GetID();
					bulletPacket.bulletPos		= ((CBullet*)pBullet)->GetPos();
					bulletPacket.bulletIsDead	= ((CBullet*)pBullet)->IsDead();
					bulletPacket.bulletDir		= ((CBullet*)pBullet)->GetDir();
					bulletPacket.bulletDegree	= ((CBullet*)pBullet)->GetDegree();

					retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
					retval = send(client_sock, reinterpret_cast<char*>(&bulletPacket), size, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send()");
						break;
					}
				}
			}
		}

		// °ÔÀÓ Å¬¸®¾î »óÅÂ¸é °ÔÀÓ Å¬¸®¾î ÆĞÅ¶ Àü¼Û
		if (IsGameClear && !isGameOver) {
			SC_GAME_CLEAR_PACKET clearPacket;
			clearPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_CLEAR);
			size = sizeof(SC_GAME_CLEAR_PACKET);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(client_sock, reinterpret_cast<char*>(&clearPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			while (1) {

			}
		}
		// °ÔÀÓ ¿À¹ö »óÅÂ¸é °ÔÀÓ ¿À¹ö ÆĞÅ¶ Àü¼Û
		else if (isGameOver && !IsGameClear) {
			SC_GAME_OVER_PACKET overPacket;
			overPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_OVER);
			size = sizeof(SC_GAME_OVER_PACKET);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(client_sock, reinterpret_cast<char*>(&overPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
		// °ÔÀÓ Å¬¸®¾î »óÅÂµµ ¾Æ´Ï°í, °ÔÀÓ ¿À¹ö »óÅÂµµ ¾Æ´Ï¸é °ÔÀÓ ÁøÇàÁß ÆĞÅ¶ Àü¼Û
		else {
			SC_GAME_PROGRESS_PACKET progressPacket;
			progressPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_PROGRESS);
			size = sizeof(SC_GAME_PROGRESS_PACKET);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(client_sock, reinterpret_cast<char*>(&progressPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
	}

	std::cout << "\n[TCP ¼­¹ö] Å¬¶óÀÌ¾ğÆ® Á¢¼Ó Á¾·á: IP ÁÖ¼Ò=" << addr << ", Æ÷Æ® ¹øÈ£=" << ntohs(clientaddr.sin_port) << ", ´Ğ³×ÀÓ=" << nick_name << std::endl;

	closesocket(client_sock);

	--clientId;

	// Å¬¶óÀÌ¾ğÆ® º¤ÅÍ¿¡¼­ ÇØ´ç Å¬¶óÀÌ¾ğÆ® Á¤º¸ »èÁ¦
	{
		std::lock_guard<std::mutex> lock{ g_mutex };
		ClientInfo.erase(std::remove_if(ClientInfo.begin(), ClientInfo.end(), [&](const PlayerInfo& info) {
			return info.id == player.id;
		}), ClientInfo.end());

		// Å¬¶óÀÌ¾ğÆ® º¤ÅÍ¿¡¼­ ÇØ´ç Å¬¶óÀÌ¾ğÆ® Á¤º¸ »èÁ¦ ÈÄ ³²Àº Å¬¶óÀÌ¾ğÆ® Á¤º¸ °»½Å
		for (int i = 0; i < ClientInfo.size(); ++i)
			ClientInfo[i].id = i;

		// ³²Àº Å¬¶óÀÌ¾ğÆ® Á¤º¸	Ãâ·Â
		std::cout<< "³²Àº Å¬¶óÀÌ¾ğÆ® Á¤º¸" << std::endl;
		for (const PlayerInfo& info : ClientInfo)
			std::cout << "id: " << info.id << ", nickname: " << info.nickname << ", portnumber: " << info.portnumber << std::endl;

		// Å¬¶óÀÌ¾ğÆ® º¤ÅÍ¿¡ ³²Àº Å¬¶óÀÌ¾ğÆ®°¡ ¾øÀ¸¸é Å¬¶óÀÌ¾ğÆ® º¤ÅÍ ÃÊ±âÈ­
		if (ClientInfo.empty())
			ClientInfo.clear();
	}
	
	delete pCharacter;
	
	return 0;
}

void init()
{
	// Å¸ÀÌ¸Ó ÃÊ±âÈ­
	CTimer::GetInst()->init();

	// ¸ó½ºÅÍ ¸¸µé±â
	CreateMonsters();

	// º¸½º ¸¸µé±â
	CreateBoss();

	// ¾ÆÀÌÅÛ ¸¸µé±â
	CreateItems();

	// Ãæµ¹ ±×·ì ÁöÁ¤
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::ITEM_RABBIT, GROUP_TYPE::PLAYER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::ITEM_COOKIE, GROUP_TYPE::PLAYER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_MONSTER, GROUP_TYPE::PLAYER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_PLAYER, GROUP_TYPE::BOSS);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_BOSS, GROUP_TYPE::PLAYER);
}

DWORD WINAPI Progress(LPVOID arg) 
{
	// ÃÊ±âÈ­
	init();

	while (1) {
		//Sleep(100 / 60);	// 600fps
		std::this_thread::sleep_for(std::chrono::milliseconds(100 / 60));
		std::lock_guard<std::mutex> lock{ g_mutex };

		// ¸Å´ÏÂ¡ ¿©±â¿¡¼­ Ã³¸®
		// Å¸ÀÌ¸Ó
		CTimer::GetInst()->update();
		
		//»èÁ¦
		CObjectMgr::GetInst()->DeleteDeadObject();

		// ¿ÀºêÁ§Æ®
		CObjectMgr::GetInst()->update();

		// Ãæµ¹
		CCollisionMgr::GetInst()->update();

		// ÀÌº¥Æ®
		CEventMgr::GetInst()->update();
	}

	return 0;
}


int main(int argc, char* argv[])
{
	int retval;

	// À©¼Ó ÃÊ±âÈ­
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ¼ÒÄÏ »ı¼º
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) 
		err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR) 
		err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) 
		err_quit("listen()");

	// µ¥ÀÌÅÍ Åë½Å¿¡ »ç¿ëÇÒ º¯¼ö
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;

	// 4¹ø ½º·¹µå
	HANDLE hThread4 = CreateThread(NULL, 0, Progress, NULL, 0, NULL);

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// Å¬¶óÀÌ¾ğÆ®¸¦ º°µµÀÇ ½º·¹µå¿¡¼­ Ã³¸®
		std::lock_guard<std::mutex> lock{ g_mutex };
		HANDLE hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);

		if (hThread == NULL)
			closesocket(client_sock); // ÇÔ¼ö ½ÇÆĞ½Ã ¼ÒÄÏ ´İ±â
		else
			CloseHandle(hThread);
	}

	CloseHandle(hThread4);

	// ¼ÒÄÏ ´İ±â
	closesocket(listen_sock);

	// À©¼Ó Á¾·á
	WSACleanup();
	return 0;
}
