#include "pch.h"
#include "CTimer.h"
#include "CObjectMgr.h"
#include "CCollisionMgr.h"
#include "CCollider.h"
#include "CEventMgr.h"

std::mutex g_mutex;
int clientId = 0;		// 클라이언트 id 초기값 0

std::vector<PlayerInfo> ClientInfo;					// 클라이언트 정보 벡터
//std::array<CPlayer, MAX_PLAYER> PlayerArray;		// 플레이어 배열

// 함수를 사용하여 클라이언트 처리
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;	// 클라이언트 소켓
	struct sockaddr_in clientaddr;		// 클라이언트 주소
	char nick_name[256];				// 클라이언트 닉네임
	char buf[BUFSIZE];					// 패킷 데이터 버퍼
	int retval;							// 리턴값
	int addrlen;						// 주소 길이
	int size;							// 패킷 사이즈
	bool isAllReady{ false };			// 모든 클라이언트가 준비 상태인지 확인
	bool isAllInit{ false };			// 모든 클라이언트가 초기화 상태인지 확인
	CObject* pCharacter = new CPlayer;

	// 접속한 클라이언트 정보 출력
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// 클라이언트의 닉네임 수신
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

	// PlayerInfo 인스턴스를 생성하여 ClientInfo 벡터에 추가
	PlayerInfo player;
	{
		std::lock_guard<std::mutex> lock{ g_mutex };		// 뮤텍스 잠금
		player.nickname = nick_name;						// 클라이언트 닉네임 저장
		player.portnumber = ntohs(clientaddr.sin_port);		// 네트워크 바이트 순서에서 호스트 바이트 순서로 변환
		player.id = clientId++;								// 클라이언트 ID 할당
		player.isReady = false;								// 클라이언트 준비 상태 false로 초기화
		player.isInit = false;								// 클라이언트 초기화 상태 false로 초기화

		// 클라이언트의 정보를 벡터에 저장(닉네임, 포트번호, 아이디)
		ClientInfo.emplace_back(player);

		// 접속한 클라이언트 정보와 닉네임 출력
		std::cout << "\nPlayer " << player.id << " 접속: IP 주소 = " << addr << ", 포트 번호 = " << ntohs(clientaddr.sin_port) << ", 닉네임 = " << nick_name << std::endl;
	}

	// 클라이언트 정보 송신
	SC_MAKE_ID_PACKET idPacket;
	size = sizeof(SC_MAKE_ID_PACKET);
	idPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
	idPacket.id = player.id;

	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(client_sock, reinterpret_cast<char*>(&idPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}

	std::cout << "[Player" << idPacket.id << "] ID 할당 패킷 전송" << std::endl;

	// 클라이언트 캐릭터 선택 정보 수신
	retval = recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
	retval = recv(client_sock, buf, size, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return 0;
	}
	else if (retval == 0)
		return 0;

	SELECT_CHARACTER_PACKET* selectPacket = reinterpret_cast<SELECT_CHARACTER_PACKET*>(buf);
	std::cout << "[Player" << player.id << "] ";
	{
		std::lock_guard<std::mutex> lock{ g_mutex };				// 뮤텍스 잠금
		((CPlayer*)pCharacter)->SetType(selectPacket->character);	// 플레이어 캐릭터 타입 설정
		((CPlayer*)pCharacter)->SetName(L"Player" + std::to_wstring(player.id));		// 플레이어 아이디에 따라서 캐릭터 이름 설정
		((CPlayer*)pCharacter)->SetPos(Vec2(0.f, 0.f));
		((CPlayer*)pCharacter)->SetDir(DIR_RIGHT);
		((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
		((CPlayer*)pCharacter)->CreateCollider();					// 충돌체 생성
		((CPlayer*)pCharacter)->GetCollider()->SetScale(Vec2(31.f, 30.f));
		ClientInfo[player.id].isReady = true;						// 클라이언트 준비 상태 true로 변경
		CObjectMgr::GetInst()->AddObject(pCharacter, GROUP_TYPE::PLAYER);			// 플레이어 객체 등록
	}

	switch (selectPacket->character) {
	case CHARACTER_TYPE::MINJI:
		std::cout << "민지 선택" << '\n';
		break;

	case CHARACTER_TYPE::HANNIE:
		std::cout << "하니 선택" << '\n';
		break;

	case CHARACTER_TYPE::DANIELLE:
		std::cout << "다니엘 선택" << '\n';
		break;

	case CHARACTER_TYPE::HEARIN:
		std::cout << "혜린 선택" << '\n';
		break;

	case CHARACTER_TYPE::HYEIN:
		std::cout << "혜인 선택" << '\n';
		break;

	default:
		break;
	}

	// 클라이언트가 3명 들어와있고, 모두 준비 상태일 경우 플레이어들에게 초기화 신호 패킷 전송
	while (1) {
		std::lock_guard<std::mutex> lock{ g_mutex };	// 뮤텍스 잠금
		isAllReady = true;

		for (const PlayerInfo& info : ClientInfo) {
			if (!info.isReady) {
				isAllReady = false;
				break;
			}
		}

		if (isAllReady && ClientInfo.size() == MAX_PLAYER)	// 모든 클라이언트가 준비 상태이면 루프 탈출
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

	std::cout << "모든 플레이어 준비 완료" << std::endl;
	std::cout << "플레이어들에게 초기화 신호 패킷 전송" << std::endl;

	// 본인을 제외한 다른 클라이언트들의 플레이어 정보 송신
	for (const PlayerInfo& info : ClientInfo) {
		if (info.id != player.id) {
			CObject* pOtherPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(info.id));

			SC_PLAYER_PACKET pp;
			pp.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
			pp.playerPos = pOtherPlayer->GetPos();
			pp.playerState = ((CPlayer*)pOtherPlayer)->GetState();
			pp.playerDir = ((CPlayer*)pOtherPlayer)->GetDir();
			pp.character = ((CPlayer*)pOtherPlayer)->GetType();
			size = sizeof(pp);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(client_sock, reinterpret_cast<char*>(&pp), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				return 0;
			}
		}
	}

	// 초기화 완료 신호 패킷 수신
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
		std::lock_guard<std::mutex> lock{ g_mutex };	// 뮤텍스 잠금
		ClientInfo[player.id].isInit = true;			// 클라이언트 초기화 상태 true로 변경
		std::cout << "[Player" << player.id << "] 스테이지 초기화 완료" << std::endl;
	}

	// 모든 클라이언트가 초기화를 완료하면 플레이어들에게 게임 시작 신호 패킷 전송
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
	size = sizeof(SC_GAME_START_PACKET);
	startPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_START);

	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(client_sock, reinterpret_cast<char*>(&startPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}

	std::cout << "모든 플레이어 초기화 완료" << std::endl;
	std::cout << "플레이어들에게 게임 시작 신호 패킷 전송" << std::endl;

	while (1) {
		Sleep(100 / 60);	// 600fps
		// 버퍼 비우기
		memset(buf, 0, BUFSIZE);

		////////////
		// recv() //
		////////////

		// 데이터 받기
		retval = recv(client_sock, reinterpret_cast<char*>(&size), sizeof(size), MSG_WAITALL);
		retval = recv(client_sock, buf, size, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		switch (buf[0]) {
			case int(CS_PACKET_TYPE::CS_KEYBOARD_INPUT):
			{
				std::lock_guard<std::mutex> lock{ g_mutex };
				CS_KEYBOARD_INPUT_PACKET* p = reinterpret_cast<CS_KEYBOARD_INPUT_PACKET*>(buf);

				for (int i = 0; i < p->keyCount; ++i) {
					// NONE이 아니면 키 입력이 들어온 것
					if (p->inputs[i].key != KEY::LAST && p->inputs[i].key_state != KEY_STATE::NONE) {
						// 플레이어 위치 자기것만 수정
						// 캐릭터 충돌 처리, 캐릭터랑 총알, 캐릭터랑 아이템
						// 몬스터랑 충돌처리
						// 각 플레이어의 위치 업데이트는 각 클라이언트의 스레드에서 배열에 동시접근해서 각자 바꿈
						Vec2 vCurPos = pCharacter->GetPos();
						Vec2 vDummyPos{};
						float speed = ((CPlayer*)pCharacter)->GetSpeed();

						// 상
						if (p->inputs[i].key == KEY::UP && p->inputs[i].key_state == KEY_STATE::HOLD) {
							vDummyPos = Vec2(vCurPos.x, vCurPos.y - speed * DT);

							if (IsInWorld(vDummyPos))
								vCurPos.y -= speed * DT;
							else if (IsInBossRoom(vDummyPos))
								vCurPos.y -= speed * DT;

							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);
						}
						if (p->inputs[i].key == KEY::UP && p->inputs[i].key_state == KEY_STATE::AWAY) {
							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
						}

						// 하
						if (p->inputs[i].key == KEY::DOWN && p->inputs[i].key_state == KEY_STATE::HOLD) {
							vDummyPos = Vec2(vCurPos.x, vCurPos.y + speed * DT);

							if (IsInWorld(vDummyPos))
								vCurPos.y += speed * DT;
							else if (IsInBossRoom(vDummyPos))
								vCurPos.y += speed * DT;

							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);
						}
						if (p->inputs[i].key == KEY::DOWN && p->inputs[i].key_state == KEY_STATE::AWAY) {
							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
						}

						// 좌
						if (p->inputs[i].key == KEY::LEFT && p->inputs[i].key_state == KEY_STATE::HOLD) {
							vDummyPos = Vec2(vCurPos.x - speed * DT, vCurPos.y);

							if (IsInWorld(vDummyPos))
								vCurPos.x -= speed * DT;
							else if (IsInBossRoom(vDummyPos))
								vCurPos.x -= speed * DT;

							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);

							if (((CPlayer*)pCharacter)->GetDir() != DIR_LEFT)
								((CPlayer*)pCharacter)->SetDir(DIR_LEFT);
						}
						if (p->inputs[i].key == KEY::LEFT && p->inputs[i].key_state == KEY_STATE::AWAY) {
							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
						}

						// 우
						if (p->inputs[i].key == KEY::RIGHT && p->inputs[i].key_state == KEY_STATE::HOLD) {
							vDummyPos = Vec2(vCurPos.x + speed * DT, vCurPos.y);

							if (IsInWorld(vDummyPos))
								vCurPos.x += speed * DT;
							else if (IsInBossRoom(vDummyPos))
								vCurPos.x += speed * DT;

							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);

							if (((CPlayer*)pCharacter)->GetDir() != DIR_RIGHT)
								((CPlayer*)pCharacter)->SetDir(DIR_RIGHT);
						}
						if (p->inputs[i].key == KEY::RIGHT && p->inputs[i].key_state == KEY_STATE::AWAY) {
							((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
						}

						if (p->inputs[i].key == KEY::SPACE && p->inputs[i].key_state == KEY_STATE::TAP) {
							//// 총알 발사
							//CreateBullet();

							//// 슈팅 이펙트 재생
							//m_EffectAnimator->Play(L"Shooting", false);
							//m_EffectAnimator->FindAnimation(L"Shooting")->SetFrame(0);
						}

						((CPlayer*)pCharacter)->SetPos(vCurPos);
					}
				}

				// 플레이어 정보 송신
				SC_PLAYER_PACKET pp;
				pp.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
				pp.playerPos = pCharacter->GetPos();
				pp.playerState = ((CPlayer*)pCharacter)->GetState();
				pp.playerDir = ((CPlayer*)pCharacter)->GetDir();
				size = sizeof(pp);

				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				retval = send(client_sock, reinterpret_cast<char*>(&pp), size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}

				// 본인을 제외한 다른 클라이언트 정보 송신
				for (const PlayerInfo& info : ClientInfo) {
					if (info.id != player.id) {
						CObject* pOtherPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(info.id));

						SC_PLAYER_PACKET pp;
						pp.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
						pp.playerPos = pOtherPlayer->GetPos();
						pp.playerState = ((CPlayer*)pOtherPlayer)->GetState();
						pp.playerDir = ((CPlayer*)pOtherPlayer)->GetDir();
						pp.character = ((CPlayer*)pOtherPlayer)->GetType();
						size = sizeof(pp);

						retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
						retval = send(client_sock, reinterpret_cast<char*>(&pp), size, 0);
						if (retval == SOCKET_ERROR) {
							err_display("send()");
							return 0;
						}
					}
				}
			}
			break;

			/*case int(CS_PACKET_TYPE::CS_SELECT_LOBBY):
			{
				CS_SELECT_LOBBY_PACKET* p = reinterpret_cast<CS_SELECT_LOBBY_PACKET*>(buf);
				std::cout << "[" << nick_name << "] 클라이언트 → 서버: 로비 버튼 선택 패킷 받음" << '\n';
			}
			break;

			case int(CS_PACKET_TYPE::CS_SELECT_EXIT):
			{
				CS_SELECT_EXIT_PACKET* p = reinterpret_cast<CS_SELECT_EXIT_PACKET*>(buf);
				std::cout << "[" << nick_name << "] 클라이언트 → 서버: 게임 종료 선택 패킷 받음" << '\n';
			}
			break;*/
		}

		////////////
		// send() //
		////////////

		// 몬스터 정보 송신
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecMonster = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::MONSTER);

			SC_MONSTER_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_MONSTER);
			size = sizeof(SC_MONSTER_PACKET);

			for (const CObject* pMonster : vecMonster) {
				// 몬스터 정보를 담는다
				p.monsterPos = ((CMonster*)pMonster)->GetPos();
				p.monsterState = ((CMonster*)pMonster)->GetState();
				p.monsterDir = ((CMonster*)pMonster)->GetDir();

				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
			}
		}
		
		// 아이템 정보 송신
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecItem = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::ITEM);

			SC_ITEM_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_ITEM);
			size = sizeof(SC_ITEM_PACKET);

			// 클라이언트에게 아이템 수 전송
			int itemCount = vecItem.size();
			retval = send(client_sock, reinterpret_cast<char*>(&itemCount), sizeof(itemCount), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			// 클라이언트에 아이템 정보 보내기
			for (CObject* pItem : vecItem) {
				p.itemPos = ((CItem*)pItem)->GetPos();
				p.itemIsDead = pItem->IsDead();

				// 클라이언트에 정보 전송
				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}

				// 아이템 삭제
				if (pItem->IsDead()) {
					// itemCount 감소
					--itemCount;
				}
			}
		}
		//// 패킷 정보 보내기 
		//switch (type) {

		//case SC_PACKET_TYPE::SC_GAME_OVER:
		//{
		//	SC_GAME_OVER_PACKET p;
		//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_OVER);
		//	size = sizeof(p);

		//	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	std::cout << "[" << nick_name << "] 서버 → 클라이언트: 게임 오버 신호 패킷 전송" << size << '\n';
		//}
		//break;

		//case SC_PACKET_TYPE::SC_GAME_CLEAR:
		//{
		//	SC_GAME_CLEAR_PACKET p;
		//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_CLEAR);
		//	size = sizeof(p);

		//	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	std::cout << "[" << nick_name << "] 서버 → 클라이언트: 게임 클리어 패킷 전송" << size << '\n';
		//}
		//break;

		//case SC_PACKET_TYPE::SC_PLAYER:
		//{
		//	SC_PLAYER_PACKET p;
		//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
		//	size = sizeof(p);

		//	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	std::cout << "[" << nick_name << "] 서버 → 클라이언트: 플레이어 정보 관련 패킷 전송" << size << '\n';
		//}
		//break;

		//case SC_PACKET_TYPE::SC_MONSTER:
		//{
		//	SC_MONSTER_PACKET p;
		//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_MONSTER);
		//	size = sizeof(p);

		//	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	std::cout << "[" << nick_name << "] 서버 → 클라이언트: 몬스터 정보 관련 패킷 전송" << size << '\n';
		//}
		//break;

		//case SC_PACKET_TYPE::SC_BOSS:
		//{
		//	SC_BOSS_PACKET p;
		//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_BOSS);
		//	size = sizeof(p);

		//	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	std::cout << "[" << nick_name << "] 서버 → 클라이언트: 보스 정보 관련 패킷 전송" << size << '\n';
		//}
		//break;

		//case SC_PACKET_TYPE::SC_BULLET:
		//{
		//	SC_BULLET_PACKET p;
		//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
		//	size = sizeof(p);

		//	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	std::cout << "[" << nick_name << "] 서버 → 클라이언트: 투사체 정보 관련 패킷 전송" << size << '\n';
		//}
		//break;

		////case SC_PACKET_TYPE::SC_ITEM:
		////{
		////	SC_ITEM_PACKET p;
		////	p.type = static_cast<char>(SC_PACKET_TYPE::SC_ITEM);
		////	size = sizeof(p);
		////	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		////	if (retval == SOCKET_ERROR) {
		////		err_display("send()");
		////		break;
		////	}
		////	retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
		////	if (retval == SOCKET_ERROR) {
		////		err_display("send()");
		////		break;
		////	}
		//// 
		////	std::cout << "[" << nick_name << "] 서버 → 클라이언트: 아이템 정보 관련 패킷을 전송하였습니다" << size << '\n';
		////}
		////break;

		//case SC_PACKET_TYPE::SC_RANK:
		//{
		//	SC_RANK_PACKET p;
		//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_RANK);
		//	size = sizeof(p);
		//	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}
		//	retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	std::cout << "[" << nick_name << "] 서버 → 클라이언트: 플레이어 순위 정보 패킷 전송" << size << '\n';
		//}
		//break;

		//default:
		//	break;
		//}

		// 데이터는 1초에 60번 전송
		//Sleep(1000 / 60);	// 60fps
	}

	std::cout << "\n[TCP 서버] 클라이언트 접속 종료: IP 주소=" << addr << ", 포트 번호=" << ntohs(clientaddr.sin_port) << ", 닉네임=" << nick_name << std::endl;

	closesocket(client_sock);

	--clientId;

	// 클라이언트 벡터에서 해당 클라이언트 정보 삭제
	{
		std::lock_guard<std::mutex> lock{ g_mutex };
		ClientInfo.erase(std::remove_if(ClientInfo.begin(), ClientInfo.end(), [&](const PlayerInfo& info) {
			return info.id == player.id;
		}), ClientInfo.end());

		// 클라이언트 벡터에서 해당 클라이언트 정보 삭제 후 남은 클라이언트 정보 갱신
		for (int i = 0; i < ClientInfo.size(); ++i)
			ClientInfo[i].id = i;

		// 남은 클라이언트 정보	출력
		std::cout<< "남은 클라이언트 정보" << std::endl;
		for (const PlayerInfo& info : ClientInfo)
			std::cout << "id: " << info.id << ", nickname: " << info.nickname << ", portnumber: " << info.portnumber << std::endl;

		// 클라이언트 벡터에 남은 클라이언트가 없으면 클라이언트 벡터 초기화
		if (ClientInfo.empty())
			ClientInfo.clear();
	}
	
	delete pCharacter;
	
	return 0;
}

void init()
{
	// 타이머 초기화
	CTimer::GetInst()->init();

	// 몬스터 만들기
	CreateMonsters();

	// 보스 만들기
	CreateBoss();

	// 아이템 만들기
	CreateItems();

	// 충돌 그룹 지정
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::ITEM, GROUP_TYPE::PLAYER);
	//CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_PLAYER, GROUP_TYPE::MONSTER);
	//CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_PLAYER, GROUP_TYPE::BOSS);
	//CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_BOSS, GROUP_TYPE::PLAYER);
	//CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_MONSTER, GROUP_TYPE::PLAYER);
	//CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MISSILE_BOSS, GROUP_TYPE::PLAYER);
}

DWORD WINAPI Progress(LPVOID arg) 
{
	// 초기화
	init();

	while (1) {
		Sleep(100 / 60);	// 600fps
		// 매니징 여기에서 처리
		// 타이머
		CTimer::GetInst()->update();
		
		//삭제
		CObjectMgr::GetInst()->DeleteDeadObject();

		// 오브젝트
		CObjectMgr::GetInst()->update();

		// 충돌
		CCollisionMgr::GetInst()->update();

		// 이벤트
		CEventMgr::GetInst()->update();
	}

	return 0;
}


int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
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

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;

	// 4번 스레드
	HANDLE hThread4 = CreateThread(NULL, 0, Progress, NULL, 0, NULL);

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 클라이언트를 별도의 스레드에서 처리
		std::lock_guard<std::mutex> lock{ g_mutex };
		HANDLE hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);

		if (hThread == NULL)
			closesocket(client_sock); // 함수 실패시 소켓 닫기
		else
			CloseHandle(hThread);
	}

	CloseHandle(hThread4);

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
