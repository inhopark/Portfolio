# CodingSkills

이 파일은 이 저장소에서 코드를 작성하거나 수정할 때 따라야 할 기본 코딩 스타일입니다.
규칙에 없는 부분은 수정하는 파일의 주변 코드 스타일을 따릅니다.

## 주석

1. Claude는 주석을 넣지 않습니다. 주석은 사용자가 직접 작성합니다.
2. 기존 주석은 지우거나 고치지 않습니다.
   - 식별자 이름을 바꿔야 하는데 그 이름을 언급하는 기존 주석이 있으면, 사용자에게 먼저 확인합니다. 허락을 받으면 해당 주석도 새 이름에 맞게 함께 고칩니다.
3. 클래스 파일 끝에 `}// end class`를 붙입니다. 주석이 아니라 파일 끝 표시이므로 새 파일을 만들 때도 붙입니다.

## 네이밍

4. 약어를 쓰지 않고 의미가 드러나게 길게 씁니다. (`PC` → `PlayerController`, `DistSq` → `DistanceSquared`)
5. bool은 `is` / `should` / `show` 등 참·거짓의 의미가 드러나는 이름으로 짓습니다.
6. 새로 만드는 enum은 이름 앞에 `E`를 붙입니다.
7. `E`가 없는 기존 enum은 사용자가 요청하기 전에는 이름을 바꾸지 않습니다.

## 메서드 이름 패턴

8. `OnXxx()` — 상태 전환 단계 (`OnReady`, `OnStart`, `OnCompleted`)
9. `OnXxxCompleted()` — 타임라인, 페이드 등 비동기 작업 완료 콜백
10. `SetXxx()` / `GetXxx()` — 값 설정 / 조회
11. `IsXxx()` / `ContainsXxx()` — bool 조회
12. `CheckXxx()` — 조건을 확인해 처리하고, 처리했는지를 bool로 반환
13. `LoadXxxUI()` / `ShowXxx()` / `CloseXxxUI()` — UI 열기 / 표시 / 닫기
14. `RegisterXxx()` / `AddXxx()` / `RemoveXxx()` / `ClearXxx()` — 등록 / 추가 / 제거 / 초기화
15. `StartXxx()` / `StopXxx()` / `PauseXxx()` / `ResumeXxx()` — 동작 제어
16. `Xxx_Success()` / `Xxx_Failure()` — 성공 / 실패 분기
17. Xxx_상황() — 같은 처리를 상황별로 나눔 (`ApplyClearState_Login`, `ApplyClearState_MapChange`)

## 서식

18. 중괄호는 항상 새 줄에 씁니다 (Allman).
19. 한 줄짜리 `if` / `for`도 반드시 `{ }`로 감쌉니다.
20. `if(`, `else if(`, `for(`는 괄호 앞에 공백을 두지 않습니다.
21. `foreach (`, `switch (`는 괄호 앞에 공백을 둡니다.
22. 관련 있는 필드끼리 붙여서 묶고, 묶음 사이에는 빈 줄을 하나 둡니다.
23. `#region` 같은 코드 접기 구역 표시는 쓰지 않습니다.

## 분기

24. enum 값에 따른 분기는 `if` / `else if` 체인으로 쓰는 것이 기본입니다.
25. `switch`를 쓸 때는 각 `case` 본문을 `{ }`로 감싸고, `break;`는 중괄호 밖에 둡니다.

```cpp
switch (State)
{
    case EState::Idle:
        {
            ...
        }
        break;
}
```

26. 조건이 맞지 않으면 함수 앞부분에서 바로 `return`합니다.
27. 삼항 연산자는 단순한 값 대입에만 쓰고, 나머지 분기는 `if` / `else`로 씁니다.

## bool 비교

28. 부정할 때 `!`를 쓰지 않고 `== false`로 씁니다.
29. bool 변수와 bool을 반환하는 함수는 항상 `== true` / `== false`로 명시합니다.

```cpp
if(bEnabled == true)
if(TargetPlayer.IsValid() == false)
```

## "시도하고 안 되면 대체 처리" 패턴

30. 할 수도 있고 안 할 수도 있는 처리는 bool을 반환하게 만듭니다.
31. 호출부에서는 결과가 `== false`일 때 대체 처리를 합니다.

```cpp
if(CheckTimelineAutoPlay(...) == false)
{
    OnAgreeTimelineComplete();
}
```

## 반복문

32. 인덱스 `for` 문은 전위 증감 `++i` / `--i`를 씁니다.
33. 순회 중에 리스트에서 항목을 삭제할 때는 역순으로 돕니다.

```cpp
for(int32 i = List.Num() - 1; i >= 0; --i)
```

## 변수와 타입

34. 타입을 생략하지 않고 명시합니다. (`auto` / `var`를 쓰지 않음)
35. 접근 제한자는 `private`까지 포함해 항상 명시합니다.
36. 필드는 선언할 때 초기값을 명시합니다. (`= nullptr`, `= 0`, `= false`)
37. null은 `if(x != nullptr)`처럼 명시적으로 비교해서 확인합니다.

## 로그

38. 로그 형식은 ## 시스템명 ## 함수명 또는 상황 ## 키 : 값입니다.
39. 경고성 로그는 일반 로그 메시지에 `Warning` / `Error` 단어를 넣습니다.
40. 진행이 불가능한 실패(데이터 없음, 생성 실패, 중복 등록)는 에러 로그로 남깁니다.
41. 상태나 설정값을 바꾸는 함수에는 바뀐 값을 남기는 로그를 넣습니다.

## 클래스 구조와 설계

42. 멤버 순서: 중첩 타입 → enum → 상수 → 프로퍼티와 필드 → 생명주기 메서드 → 기능별 메서드 묶음 → 작은 private 헬퍼(파일 끝)
43. 생명주기 메서드는 실제 호출 순서대로 배치합니다. (`Initialize` → `OnReady` → `OnStart` → `OnProgress` → `OnCompleted` → `Success` / `Fail` → `FinalizeResult`)
44. 외부에는 읽기만 허용하고, 값 변경은 클래스 내부에서만 합니다.
45. 상태 변경은 한 함수(`SetXxxState`)에서만 합니다.
46. 상태 변경 함수는 변경에 성공했는지를 bool로 반환합니다.
47. 부모 클래스는 자식이 채울 `protected virtual` 빈 함수를 제공합니다.
48. `override`할 때는 부모 함수를 먼저 호출한 뒤 자기 로직을 추가합니다.
49. 동작 방식은 enum 타입 필드로 설정하게 합니다.
50. 종류별 객체 생성은 enum → 생성 함수 매핑에 등록합니다.
51. 같은 종류를 중복 등록하려 하면 막고 에러 로그를 남깁니다.

## C++(언리얼) 적용 기준

위 규칙을 C++ / Unreal Engine 코드에 적용할 때의 기준입니다.

- 파일 끝 표시(3번)는 헤더의 클래스 선언 끝에 `};// end class`로 붙입니다. 클래스가 없는 헤더와 `.cpp` 파일에는 붙이지 않습니다.
- C++에는 `foreach` 키워드가 없으므로 범위 기반 for 문도 `for(`로 씁니다. (`for(AActor* Actor : FoundActors)`)
- 헤더의 한 줄짜리 인라인 getter(`FORCEINLINE float GetHP() const { return HP; }`)는 18번(Allman)의 예외로 한 줄로 씁니다.
- null 확인(37번)은 `if(UWorld* World = GetWorld())`처럼 조건문 안에서 선언하지 않고, 변수를 먼저 선언한 뒤 `if(World != nullptr)` / `if(World == nullptr) { return; }`로 비교합니다.
- 필드 초기값(36번)은 포인터(`= nullptr`), 숫자, bool, enum 필드에 명시합니다. `TArray`, `TWeakObjectPtr`, `FTimerHandle`처럼 기본 생성자가 있는 타입은 생략합니다.
- 로그는 `UE_LOG`를 쓰고, 진행이 불가능한 실패는 `Error` verbosity로 남깁니다. enum 값은 `*UEnum::GetValueAsString(Value)`, bool 값은 `*LexToString(bValue)`로 출력합니다.
- bool 이름(5번)은 언리얼 관례대로 `b` 접두사를 붙이고, 그 뒤에 `Is` / `Should` / `Show` 등 의미가 드러나는 단어를 씁니다. (`bIsEnabled`, `bShouldRestore`)
