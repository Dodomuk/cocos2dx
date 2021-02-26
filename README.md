COCOS2DX로 구현한 2D 비행기 게임 

------------------------------------------------------------------------------------------
     어렸을적 90년대 생이라면 오락실에서 한번쯤은 이 비행기 게임을 해본 적이 있을것이다.
     이제는 흔히 찾아 볼 수 없는 이 추억을 직접 우리 손으로 구현해보자!
------------------------------------------------------------------------------------------     
     
     -Play / Quit 메뉴
     -BGM 배경음악 설정
     
     
     
     *PLAY
     
     -게임 시작화면으로 전환
      -BGM 배경음악 설정
      
     -사용자(비행기)는 총알을 쏘면서 등장. 총알로 위 아래 일정한 속도 조절
     -마우스 클릭으로 비행기를 좌,우,위, 아래로 조절 가능
     
     -총 생명(상단 왼쪽 LIFE)은 3으로 지정
     -적 비행기와 부딫힐 경우 -1
     
     -총 점수(상단 오른쪽 SCORE)는 현재 점수와 최고 기록이 적혀 있음 (ex. SCORE 30 / 450 [현재점수 / 총점수])
     -초록 우주선 격파시 +1, 빨간 우주선 격파시 +5
     
     -초록 우주선의 HP 1로 설정
     -빨간 우주선 HP 5로 설정 (사용자 총알은 1발당 1씩 깎는다)
     
     -바닥에 움직이는 아이템을 먹게 될 경우 총알 강화
     -강화된 총알의 경우 적 우주선의 색과 관계 없이 1방에 처리 가능
     
     -파티클 설정
     
     -적 비행기와 사용자의 비행기 충돌시 게임 종료
     -GAMEOVER 화면이 뜨면서 화면 멈춤
     -마우스 클릭시 다시 MENU화면으로 전환
     
     
     
     *QUIT
     -게임 종료, 화면 창 꺼짐