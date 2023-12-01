#include "pch.h"
#include "CTimer.h"
#include "CObjectMgr.h"
#include "CCollisionMgr.h"
#include "CCollider.h"
#include "CEventMgr.h"
#include "CBullet.h"

std::mutex g_mutex;
int clientId = 0;		// 클라이언트 id 초기값 0

std::vector<PlayerInfo> ClientInfo;					// 클라이언트 정보 벡터

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

	// 접속한 클라이언트 정보 저장
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
	idPacket.type	= static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
	idPacket.id		= player.id;
	size = sizeof(SC_MAKE_ID_PACKET);

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

	SELECT_CHARACTER_PACKET* pSelectPacket = reinterpret_cast<SELECT_CHARACTER_PACKET*>(buf);
	{
		std::lock_guard<std::mutex> lock{ g_mutex };
		((CPlayer*)pCharacter)->SetType(pSelectPacket->character);
		((CPlayer*)pCharacter)->SetName(L"Player" + std::to_wstring(player.id));
		((CPlayer*)pCharacter)->SetPos(Vec2(0.f, 0.f));
		((CPlayer*)pCharacter)->SetDir(DIR_RIGHT);
		((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
		((CPlayer*)pCharacter)->CreateCollider();
		((CPlayer*)pCharacter)->GetCollider()->SetScale(Vec2(31.f, 30.f));

		ClientInfo[player.id].isReady = true;	// 클라이언트 준비 상태 true로 변경
		CObjectMgr::GetInst()->AddObject(pCharacter, GROUP_TYPE::PLAYER);	// 플레이어 객체 등록
	}

	switch (pSelectPacket->character) {
	case CHARACTER_TYPE::MINJI:
		std::cout << "[Player" << player.id << "] " << "민지 선택" << std::endl;
		break;

	case CHARACTER_TYPE::HANNIE:
		std::cout << "[Player" << player.id << "] " << "하니 선택" << std::endl;
		break;

	case CHARACTER_TYPE::DANIELLE:
		std::cout << "[Player" << player.id << "] " << "다니엘 선택" << std::endl;
		break;

	case CHARACTER_TYPE::HEARIN:
		std::cout << "[Player" << player.id << "] " << "혜린 선택" << std::endl;
		break;

	case CHARACTER_TYPE::HYEIN:
		std::cout << "[Player" << player.id << "] " << "혜인 선택" << std::endl;
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
	startPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_START);
	size = sizeof(SC_GAME_START_PACKET);

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

		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			//받아온 패킷을 키보드 입력 패킷으로 캐스팅
			CS_KEYBOARD_INPUT_PACKET* pKeyInputPacket = reinterpret_cast<CS_KEYBOARD_INPUT_PACKET*>(buf);

			for (int i = 0; i < pKeyInputPacket->keyCount; ++i) {
				// NONE이 아닐 때만	처리
				if (pKeyInputPacket->inputs[i].key != KEY::LAST && pKeyInputPacket->inputs[i].key_state != KEY_STATE::NONE) {
					// 플레이어 위치 자기것만 수정
					// 각 플레이어의 위치 업데이트는 각 클라이언트의 스레드에서 배열에 동시접근해서 각자 바꿈
					Vec2 vCurPos = pCharacter->GetPos();
					Vec2 vDummyPos{};
					float speed = ((CPlayer*)pCharacter)->GetSpeed();

					// 상
					if (pKeyInputPacket->inputs[i].key == KEY::UP && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
						vDummyPos = Vec2(vCurPos.x, vCurPos.y - speed * DT);

						if (IsInWorld(vDummyPos))
							vCurPos.y -= speed * DT;
						else if (IsInBossRoom(vDummyPos))
							vCurPos.y -= speed * DT;

						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);
					}
					if (pKeyInputPacket->inputs[i].key == KEY::UP && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
					}

					// 하
					if (pKeyInputPacket->inputs[i].key == KEY::DOWN && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
						vDummyPos = Vec2(vCurPos.x, vCurPos.y + speed * DT);

						if (IsInWorld(vDummyPos))
							vCurPos.y += speed * DT;
						else if (IsInBossRoom(vDummyPos))
							vCurPos.y += speed * DT;

						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);
					}
					if (pKeyInputPacket->inputs[i].key == KEY::DOWN && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
					}

					// 좌
					if (pKeyInputPacket->inputs[i].key == KEY::LEFT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
						vDummyPos = Vec2(vCurPos.x - speed * DT, vCurPos.y);

						if (IsInWorld(vDummyPos))
							vCurPos.x -= speed * DT;
						else if (IsInBossRoom(vDummyPos))
							vCurPos.x -= speed * DT;

						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);

						if (((CPlayer*)pCharacter)->GetDir() != DIR_LEFT)
							((CPlayer*)pCharacter)->SetDir(DIR_LEFT);
					}
					if (pKeyInputPacket->inputs[i].key == KEY::LEFT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
					}

					// 우
					if (pKeyInputPacket->inputs[i].key == KEY::RIGHT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::HOLD) {
						vDummyPos = Vec2(vCurPos.x + speed * DT, vCurPos.y);

						if (IsInWorld(vDummyPos))
							vCurPos.x += speed * DT;
						else if (IsInBossRoom(vDummyPos))
							vCurPos.x += speed * DT;

						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::RUN);

						if (((CPlayer*)pCharacter)->GetDir() != DIR_RIGHT)
							((CPlayer*)pCharacter)->SetDir(DIR_RIGHT);
					}
					if (pKeyInputPacket->inputs[i].key == KEY::RIGHT && pKeyInputPacket->inputs[i].key_state == KEY_STATE::AWAY) {
						((CPlayer*)pCharacter)->SetState(PLAYER_STATE::IDLE);
					}

					if (pKeyInputPacket->inputs[i].key == KEY::SPACE && pKeyInputPacket->inputs[i].key_state == KEY_STATE::TAP) {
						// 총알 발사
						((CPlayer*)pCharacter)->CreateBullet(player.id);
					}

					// 처리한 정보 업데이트
					((CPlayer*)pCharacter)->SetPos(vCurPos);
				}
			}

			// 플레이어 정보 송신
			SC_PLAYER_PACKET playerPacket;
			playerPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
			playerPacket.playerID = player.id;
			playerPacket.playerPos = pCharacter->GetPos();
			playerPacket.playerState = ((CPlayer*)pCharacter)->GetState();
			playerPacket.playerDir = ((CPlayer*)pCharacter)->GetDir();
			size = sizeof(playerPacket);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			retval = send(client_sock, reinterpret_cast<char*>(&playerPacket), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			// 본인을 제외한 다른 클라이언트 정보 송신
			for (const PlayerInfo& otherPlayerInfo : ClientInfo) {
				if (otherPlayerInfo.id != player.id) {
					CObject* pOtherPlayer = CObjectMgr::GetInst()->FindObject(L"Player" + std::to_wstring(otherPlayerInfo.id));

					SC_PLAYER_PACKET otherPlayerPacket;
					otherPlayerPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
					otherPlayerPacket.playerID = otherPlayerInfo.id;
					otherPlayerPacket.playerPos = pOtherPlayer->GetPos();
					otherPlayerPacket.playerState = ((CPlayer*)pOtherPlayer)->GetState();
					otherPlayerPacket.playerDir = ((CPlayer*)pOtherPlayer)->GetDir();
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

		// 몬스터 정보 송신
		{
			std::lock_guard<std::mutex> lock{ g_mutex };
			const std::vector<CObject*>& vecMonster = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::MONSTER);
			
			SC_MONSTER_PACKET monsterPacket;
			monsterPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_MONSTER);
			size = sizeof(SC_MONSTER_PACKET);

			for (const CObject* pMonster : vecMonster) {
				// 몬스터 정보를 담는다
				monsterPacket.monsterPos	= ((CMonster*)pMonster)->GetPos();
				monsterPacket.monsterState	= ((CMonster*)pMonster)->GetState();
				monsterPacket.monsterDir	= ((CMonster*)pMonster)->GetDir();

				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				retval = send(client_sock, reinterpret_cast<char*>(&monsterPacket), size, 0);
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

			SC_ITEM_PACKET itemPacket;
			itemPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_ITEM);
			size = sizeof(SC_ITEM_PACKET);

			// 아이템 수 전송
			int itemCount = vecItem.size();
			retval = send(client_sock, reinterpret_cast<char*>(&itemCount), sizeof(itemCount), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			// 아이템 정보 보내기
			for (CObject* pItem : vecItem) {
				itemPacket.itemPos		= ((CItem*)pItem)->GetPos();
				itemPacket.itemIsDead	= pItem->IsDead();

				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				retval = send(client_sock, reinterpret_cast<char*>(&itemPacket), size, 0);
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
		
		// 플레이어 투사체 정보 송신
		//{
		//	std::lock_guard<std::mutex> lock{ g_mutex };
		//	const std::vector<CObject*>& vecBullet = CObjectMgr::GetInst()->GetGroupObject(GROUP_TYPE::BULLET_PLAYER);

		//	SC_BULLET_PACKET p;
		//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
		//	size = sizeof(SC_BULLET_PACKET);

		//	// 클라이언트에게 플레이어 투사체 수 전송
		//	int bulletCount = vecBullet.size();
		//	retval = send(client_sock, reinterpret_cast<char*>(&bulletCount), sizeof(bulletCount), 0);
		//	if (retval == SOCKET_ERROR) {
		//		err_display("send()");
		//		break;
		//	}

		//	// 클라이언트에 플레이어 투사체 정보 보내기
		//	for (CObject* pBullet : vecBullet) {
		//		p.bulletPos = ((CBullet*)pBullet)->GetPos();
		//		p.bulletIsDead = ((CBullet*)pBullet)->IsDead();
		//		p.bulletDir = ((CBullet*)pBullet)->GetDir();

		//		retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
		//		retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
		//		if (retval == SOCKET_ERROR) {
		//			err_display("send()");
		//			break;
		//		}

		//		// 투사체 삭제
		//		if (pBullet->IsDead()) {
		//			// BulletCount 감소
		//			--bulletCount;
		//		}
		//	}
		//}
		
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
