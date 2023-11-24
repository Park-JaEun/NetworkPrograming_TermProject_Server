#include "pch.h"
#include "define.h"
#include "CTimer.h"

int clientId = 0;  // 클라이언트 id 초기값 0

std::vector<PlayerInfo> ClientInfo;
std::array<CPlayer, 3> PlayerArray;

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
	player.nickname = nick_name;						// 클라이언트 닉네임 저장
	player.portnumber = ntohs(clientaddr.sin_port);		// 네트워크 바이트 순서에서 호스트 바이트 순서로 변환
	player.id = clientId;								// 클라이언트 닉네임 저장
	player.isReady = false;								// 클라이언트 준비 상태 false로 초기화
	player.isInit = false;								// 클라이언트 초기화 상태 false로 초기화

	// 클라이언트의 정보를 벡터에 저장(닉네임, 포트번호, 아이디)
	ClientInfo.emplace_back(player);

	// 접속한 클라이언트 정보와 닉네임 출력
	std::cout << "\nPlayer " << clientId << " 접속: IP 주소 = " << addr << ", 포트 번호 = " << ntohs(clientaddr.sin_port) << ", 닉네임 = " << nick_name << std::endl;

	// 클라이언트 정보 송신
	SC_MAKE_ID_PACKET idPacket;
	size = sizeof(SC_MAKE_ID_PACKET);
	idPacket.type = static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
	idPacket.id = clientId;

	retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
	retval = send(client_sock, reinterpret_cast<char*>(&idPacket), size, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return 0;
	}

	std::cout << "[Player" << clientId << "] ID 할당 패킷 전송" << std::endl;

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
	std::cout << "[Player" << clientId << "] ";
	ClientInfo[clientId].isReady = true;	// 클라이언트 준비 상태 true로 변경

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

	// 모든 클라이언트가 준비 상태인지 확인 (동기화)
	while (1) {
		isAllReady = true;

		for (const PlayerInfo& info : ClientInfo) {
			if (!info.isReady) {
				isAllReady = false;
				break;
			}
		}

		if (isAllReady)
			break;
	}

	// 모든 클라이언트가 준비 상태일 경우 플레이어들에게 초기화 신호 패킷 전송
	if (isAllReady) {
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
	std::cout << "[Player" << clientId << "] 스테이지 초기화 완료" << std::endl;
	ClientInfo[clientId].isInit = true;	// 클라이언트 초기화 상태 true로 변경

	// 모든 클라이언트가 초기화를 완료하면 플레이어들에게 게임 시작 신호 패킷 전송
	while (1) {
		isAllInit = true;

		for (const PlayerInfo& info : ClientInfo) {
			if (!info.isInit) {
				isAllInit = false;
				break;
			}
		}

		if (isAllInit)
			break;
	}

	if (isAllInit) {
		Sleep(3000);	// 3초 대기
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
	}

	CTimer::GetInst()->init();

	while (1) {

		CTimer::GetInst()->update();

		////////////
		// recv() //
		////////////
		
		// 버퍼 비우기
		memset(buf, 0, BUFSIZE);

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
				CS_KEYBOARD_INPUT_PACKET* p = reinterpret_cast<CS_KEYBOARD_INPUT_PACKET*>(buf);
				std::cout << "[" << nick_name << "] 클라이언트 → 서버: 키보드 입력 정보 패킷 받음" << '\n';
				
				// 수신한 키보드 정보와 상태 출력
				std::cout << (int)p->key << std::endl;
				std::cout << (int)p->key_state << std::endl;

				// 플레이어 위치 자기것만 수정
				// 캐릭터 충돌 처리, 캐릭터랑 총알, 캐릭터랑 아이템
				// 몬스터랑 충돌처리
				// 각 플레이어의 위치 업데이트는 각 클라이언트의 스레드에서 배열에 동시접근해서 각자 바꿈
				Vec2 vCurPos = PlayerArray[clientId].GetPos();
				float speed = PlayerArray[clientId].GetSpeed();

				// 상
				if (p->key == KEY::UP && p->key_state == KEY_STATE::HOLD) {
					/*vDummyPos = Vec2(vPos.x, vPos.y - m_fSpeed * DT);*/

					//if (IsInWorld(vDummyPos) && !bIsBoss)
					//	vPos.y -= m_fSpeed * DT;
					//else if (IsInBossRoom(vDummyPos) && bIsBoss)
					//	vPos.y -= m_fSpeed * DT;

					if (IsInWorld(vCurPos))
						vCurPos.y -= speed * DT;
					else if (IsInBossRoom(vCurPos) )
						vCurPos.y -= speed * DT;

					PlayerArray[clientId].SetState(PLAYER_STATE::RUN);
				}
				if (p->key == KEY::UP && p->key_state == KEY_STATE::AWAY) {
					PlayerArray[clientId].SetState(PLAYER_STATE::IDLE);
				}

				// 하
				if (p->key == KEY::DOWN && p->key_state == KEY_STATE::HOLD) {
					/*vDummyPos = Vec2(vPos.x, vPos.y + m_fSpeed * DT);

					if (IsInWorld(vDummyPos) && !bIsBoss)
						vPos.y += m_fSpeed * DT;
					else if (IsInBossRoom(vDummyPos) && bIsBoss)
						vPos.y += m_fSpeed * DT;

					m_eState = PLAYER_STATE::RUN;*/
					if (IsInWorld(vCurPos))
						vCurPos.y += speed * DT;
					else if (IsInBossRoom(vCurPos))
						vCurPos.y += speed * DT;

					PlayerArray[clientId].SetState(PLAYER_STATE::RUN);
				}
				if (p->key == KEY::DOWN && p->key_state == KEY_STATE::AWAY) {
					PlayerArray[clientId].SetState(PLAYER_STATE::IDLE);
				}

				// 좌
				if (p->key == KEY::LEFT && p->key_state == KEY_STATE::HOLD) {
					/*vDummyPos = Vec2(vPos.x - m_fSpeed * DT, vPos.y);

					if (IsInWorld(vDummyPos) && !bIsBoss)
						vPos.x -= m_fSpeed * DT;
					else if (IsInBossRoom(vDummyPos) && bIsBoss)
						vPos.x -= m_fSpeed * DT;

					m_eState = PLAYER_STATE::RUN;
					if (m_bDir != DIR_LEFT)
						m_bDir = DIR_LEFT;*/

					if (IsInWorld(vCurPos))
						vCurPos.x -= speed * DT;
					else if (IsInBossRoom(vCurPos))
						vCurPos.x -= speed * DT;

					PlayerArray[clientId].SetState(PLAYER_STATE::RUN);
					if (PlayerArray[clientId].GetDir() != DIR_LEFT)
						PlayerArray[clientId].SetDir(DIR_LEFT);
				}
				if (p->key == KEY::LEFT && p->key_state == KEY_STATE::AWAY) {
					PlayerArray[clientId].SetState(PLAYER_STATE::IDLE);
				}

				// 우
				if (p->key == KEY::RIGHT && p->key_state == KEY_STATE::HOLD) {
					/*vDummyPos = Vec2(vPos.x + m_fSpeed * DT, vPos.y);

					if (IsInWorld(vDummyPos) && !bIsBoss)
						vPos.x += m_fSpeed * DT;
					else if (IsInBossRoom(vDummyPos) && bIsBoss)
						vPos.x += m_fSpeed * DT;

					m_eState = PLAYER_STATE::RUN;
					if (m_bDir != DIR_RIGHT)
						m_bDir = DIR_RIGHT;*/

					if (IsInWorld(vCurPos))
						vCurPos.x += speed * DT;
					else if (IsInBossRoom(vCurPos))
						vCurPos.x += speed * DT;

					PlayerArray[clientId].SetState(PLAYER_STATE::RUN);
					if (PlayerArray[clientId].GetDir() != DIR_RIGHT)
						PlayerArray[clientId].SetDir(DIR_RIGHT);
				}
				if (p->key == KEY::RIGHT && p->key_state == KEY_STATE::AWAY) {
					PlayerArray[clientId].SetState(PLAYER_STATE::IDLE);
				}

				if (p->key == KEY::SPACE && p->key_state == KEY_STATE::TAP) {
					//// 총알 발사
					//CreateBullet();

					//// 슈팅 이펙트 재생
					//m_EffectAnimator->Play(L"Shooting", false);
					//m_EffectAnimator->FindAnimation(L"Shooting")->SetFrame(0);
				}
		
				PlayerArray[clientId].SetPos(vCurPos);

				// 바로 send 플레이어 인포
				// 플레이어 인포 3개짜리 배열을 보냄
				SC_PLAYER_PACKET pp;
				pp.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
				pp.playerPos = PlayerArray[clientId].GetPos();
				pp.playerState = PlayerArray[clientId].GetState();
				pp.playerDir = PlayerArray[clientId].GetDir();
				size = sizeof(pp);

				retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}

				retval = send(client_sock, reinterpret_cast<char*>(&pp), size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}

				std::cout << "[" << nick_name << "] 서버 → 클라이언트: 플레이어 정보 관련 패킷 전송" << size << '\n';
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
		SC_PACKET_TYPE type = SC_PACKET_TYPE(rand() % 11 + 1);

		// 패킷 정보 보내기 
		switch (type) {
		case SC_PACKET_TYPE::SC_MAKE_ID:
		{
			SC_MAKE_ID_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_MAKE_ID);
			size = sizeof(p);
			
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] 서버 → 클라이언트: ID 할당 관련 패킷 전송" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SELECT_CHARACTER:
		{
			SELECT_CHARACTER_PACKET p;

			p.type = static_cast<char>(SC_PACKET_TYPE::SELECT_CHARACTER);
			size = sizeof(p);

			//여기서 변경해서 보내면 돼
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] 서버 → 클라이언트: 캐릭터 선택 관련 패킷 전송" <<size<< '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_INIT:
		{
			SC_INIT_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_INIT);
			size = sizeof(p);
			
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] 서버 → 클라이언트: 초기화 요청 신호 관련 패킷 전송" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_GAME_START:
		{
			SC_GAME_START_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_START);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] 서버 → 클라이언트: 게임 시작 신호 패킷 전송" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_GAME_OVER:
		{
			SC_GAME_OVER_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_OVER);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] 서버 → 클라이언트: 게임 오버 신호 패킷 전송" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_GAME_CLEAR:
		{
			SC_GAME_CLEAR_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_GAME_CLEAR);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] 서버 → 클라이언트: 게임 클리어 패킷 전송" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_PLAYER:
		{
			SC_PLAYER_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_PLAYER);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] 서버 → 클라이언트: 플레이어 정보 관련 패킷 전송" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_MONSTER:
		{
			SC_MONSTER_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_MONSTER);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] 서버 → 클라이언트: 몬스터 정보 관련 패킷 전송" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_BOSS:
		{
			SC_BOSS_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_BOSS);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] 서버 → 클라이언트: 보스 정보 관련 패킷 전송" << size << '\n';
		}
		break;

		case SC_PACKET_TYPE::SC_BULLET:
		{
			SC_BULLET_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_BULLET);
			size = sizeof(p);

			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] 서버 → 클라이언트: 투사체 정보 관련 패킷 전송" << size << '\n';
		}
		break;

		//case SC_PACKET_TYPE::SC_ITEM:
		//{
		//	SC_ITEM_PACKET p;
		//	p.type = static_cast<char>(SC_PACKET_TYPE::SC_ITEM);
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
		// 
		//	std::cout << "[" << nick_name << "] 서버 → 클라이언트: 아이템 정보 관련 패킷을 전송하였습니다" << size << '\n';
		//}
		//break;

		case SC_PACKET_TYPE::SC_RANK:
		{
			SC_RANK_PACKET p;
			p.type = static_cast<char>(SC_PACKET_TYPE::SC_RANK);
			size = sizeof(p);
			retval = send(client_sock, reinterpret_cast<char*>(&size), sizeof(size), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			retval = send(client_sock, reinterpret_cast<char*>(&p), size, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			std::cout << "[" << nick_name << "] 서버 → 클라이언트: 플레이어 순위 정보 패킷 전송" << size << '\n';
		}
		break;

		default:
			break;
		}

		// 데이터는 1초에 60번 전송
		//Sleep(1000 / 60);	// 60fps
	}

	std::cout << "\n[TCP 서버] 클라이언트 접속 종료: IP 주소=" << addr << ", 포트 번호=" << ntohs(clientaddr.sin_port) << ", 닉네임=" << nick_name << std::endl;

	closesocket(client_sock);

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

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		++clientId;	// 클라이언트 id 설정

		// 클라이언트를 별도의 스레드에서 처리
		HANDLE hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);

		if (hThread == NULL)
			closesocket(client_sock); // 함수 실패시 소켓 닫기
		else
			CloseHandle(hThread);
	}

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
