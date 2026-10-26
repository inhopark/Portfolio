# MissionSystem

Unreal Engine 5.8 + C++로 제작한 **NPC 기반 미션 시스템** 포트폴리오 프로젝트입니다.
NPC에게 미션을 수락하면 위에서 몬스터가 내려오는 **쿼터뷰 디펜스 미니게임**으로 진입해 일정 시간을 생존하면 성공하는 흐름을 보여줍니다. 완성된 게임이 아니라, 깔끔한 코드 구조와 디자인 패턴 적용을 보여주는 데 목적이 있습니다.

## 진행 흐름

1. 플레이어가 NPC와 오버랩 → 미션 수락/거절 대화창 표시
2. 수락 시 쿼터뷰 디펜스 모드로 전환, 몬스터 웨이브 스폰 + 생존 타이머 시작
3. 몬스터를 피하며 생존 → 타이머 만료 시 성공, HP 0 도달 시 실패
4. 결과창 확인 → HP 회복, 원래 위치로 복귀, NPC 재활성화

## 자동 테스트 사이클

매번 NPC까지 걸어가 버튼을 눌러가며 전체 흐름을 확인하는 대신, 미션 사이클(NPC 접근 → 수락 → 디펜스 미니게임 → 결과 확인) 전체를 자동으로 한 번 돌려 시스템이 정상 동작하는지 검증할 수 있는 `UMissionAutoCycleController`를 두었습니다. 별도 UI나 입력 액션 없이 콘솔 명령(`ToggleAutoCycleMode`)으로 켜고 끄며, `MissionManager`가 노출하는 기존 public 함수를 사람이 버튼을 누른 것과 동일한 방식으로 호출하는 구조라 실제 게임 로직 코드는 전혀 건드리지 않습니다. 자동화된 테스트 러너가 없는 프로젝트에서, 반복 수동 플레이 없이 전체 플로우의 회귀를 빠르게 확인하기 위한 최소한의 장치입니다.

## 설계 포인트

적용한 패턴 위주로 정리했습니다.

| 패턴 | 적용 위치 |
|---|---|
| State Machine | `UBaseMission` — `Ready → InProgress → Succeeded/Failed` |
| Factory Method | `UMissionFactory` — `EMissionUnique` → 미션 인스턴스 매핑 |
| Template Method | `UDefenseMinigameMission` → `UMissionAct1Chapter1/2` |
| Object Pool | `AMonster` — `Destroy()` 대신 `Deactivate()`로 풀에 반환 |
| Facade | `UMissionManager` — HUD/위젯이 서브시스템 하나만 알면 되도록 진입점 통합 |

## 기술 스택

- Unreal Engine 5.8, C++
- Enhanced Input
- UMG (State 기반 HUD/대화창 위젯)

## 빌드 / 실행

이 저장소에는 `.sln`/`.vcxproj`가 포함되어 있지 않습니다(gitignore). 클론 후:

1. `MissionSystem.uproject` 우클릭 → **Generate Visual Studio project files**
2. 생성된 `.sln`을 Visual Studio 2022(Unreal 워크로드)에서 열어 `MissionSystemEditor` (Development Editor, Win64) 빌드
3. 또는 `MissionSystem.uproject`를 바로 실행해 컴파일 여부를 물으면 승인

코드 스타일 규칙은 [`ClaudeSkills/CodingSkills.md`](./ClaudeSkills/CodingSkills.md)를 참고하세요.

## AI(Claude Code) 활용

개발 과정에서 Claude Code를 페어 프로그래밍 도구로 활용했습니다. AI에게 매번 같은 요구사항을 반복해서 설명하는 대신, **지켜야 할 규칙을 파일로 정리해 AI가 항상 같은 기준으로 작업하도록** 만드는 데 초점을 두었습니다.

### 규칙 파일 구조

```
CLAUDE.md                  ← Claude Code가 세션 시작 시 자동으로 읽는 진입점
ClaudeSkills/
├─ DefaultSkills.md        ← 공통 작업 규칙 (파일 수정 방식, 빌드·검증 절차, 언리얼 프로젝트 파일 재생성 등)
├─ CodingSkills.md         ← 나만의 코딩 스타일 규칙 51개 + 언어별(C++ / C#) 코딩 표준 적용 기준
├─ TokenSavingSkills.md    ← 토큰 사용을 줄이는 규칙 (답변 방식, 파일 읽기, 명령 출력 확인)
└─ PortfolioSkills.md      ← 이력서/포트폴리오 PDF 수정·생성 절차 (해당 작업 시에만 읽음)
```

`CLAUDE.md`에는 규칙 자체를 적지 않고, `@ClaudeSkills/...` import로 어떤 규칙 파일을 참고할지만 적었습니다. import한 규칙 파일은 세션 시작 시 자동으로 함께 로드되므로, AI가 규칙을 빠뜨리지 않고 처음부터 적용한 상태로 작업합니다. 규칙의 종류별로 파일을 나눠 두어 다른 프로젝트에서도 필요한 파일만 골라 재사용할 수 있습니다.

- 모든 규칙에는 `섹션.순서` 형식의 번호(예: `DefaultSkills 4.2`)를 붙여, 번호로 규칙을 찾아 묻거나 새 규칙을 추가하기 쉽게 했습니다.
- import한 파일은 세션마다 전부 로드되어 토큰을 쓰므로, 포트폴리오 작업처럼 특정 작업에만 필요한 `PortfolioSkills.md`는 import하지 않고 경로만 적어 두어 그 작업을 할 때만 읽도록 했습니다.

### 나만의 코딩 스타일 적용

`CodingSkills.md`는 **나의 코딩 스타일**을 정리한 규칙 파일입니다. 네이밍 표기법처럼 언어마다 정해진 부분은 C++ / C# 각 언어의 코딩 표준을 따르고, 그 위에 내 코딩 스타일 규칙을 적용하도록 해서 어떤 언어로 작업하든 AI가 작성하는 코드가 내 스타일을 그대로 따르도록 했습니다.

- 불리언은 `!` 대신 `== true` / `== false`로 명시적으로 비교
- 조건이 맞지 않으면 함수 앞부분에서 바로 `return`
- 상태 변경은 한 함수에서만 하고, 변경 성공 여부를 bool로 반환
- 로그는 `## 시스템 ## 함수 ## 키 : 값` 형식으로 통일
- Claude는 주석을 직접 넣지 않고, 기존 주석도 임의로 고치지 않음

이 규칙을 정리한 뒤 Claude Code에게 기존 소스 전체(37개 파일)를 규칙에 맞게 정리하도록 맡겼습니다. Claude Code는 수정 후 일반 빌드와 non-unity 빌드를 모두 통과시켰고, 게임을 실행해 자동 테스트 사이클(`ToggleAutoCycleMode`)로 미션 전체 흐름이 그대로 동작하는지 로그로 검증했습니다. 규칙끼리 충돌하는 부분(예: 함수 이름을 바꾸면 그 이름을 언급하는 주석이 틀려지지만, 주석은 고치지 않는다는 규칙)은 임의로 처리하지 않고 판단을 요청하도록 했습니다.

### 규칙을 계속 쌓아 가는 방식

- 작업 중 반복해서 지켜야 할 내용이 생기면 Claude가 판단해 해당 Skills 파일에 규칙으로 추가하고, 무엇을 추가했는지 항상 사용자에게 알리도록 했습니다.
- 진입점인 `CLAUDE.md`는 사용자 승인 없이는 수정하지 않도록 했습니다.
- 이렇게 쌓인 규칙은 다음 작업부터 자동으로 적용되므로, 같은 실수를 반복해서 바로잡을 필요가 줄고 AI 활용도가 점점 높아집니다.
