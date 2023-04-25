0. 구성
 - [project2]20181297.pdf : 보고서
 - 20181297.png: logical schema diagram
 - 20181297.erwin: physical schema diagram
 - 20181297.zip: ODBC project 파일. 
		project2.sln을 통해 project를 open. resource로 20181297.txt, source code로 20181297.cpp가 있다.

1. database connection 변수
 - host, user, pw, db는 databse에 connection 하기 위한 변수로 쓰인다.
 - 각각의 변수를 접속하고자하는 server 정보에 따라 설정한 뒤 connection을 이행할 것.

2. CRUD txt 파일
 - 20181297.txt 이름으로 create, insert, drop을 이행하는 sql문이 들어있다.
 - cpp 파일 초반부 파일명 수정시 다른 파일 또한 사용할 수 있다.

3. Type 1 입력
 - 차례로 택배사에 'USPS', 송장번호에 '9375-8727-1163137'를 입력한다.

4. Type 3-1, 4-1 입력
 - k에 보고싶은 data 개수를 입력 ex) 3, 5 etc.

5. 입력 예외 처리
 - type의 경우 0~7 이외의 숫자가 들어오면 0~7 사이의 숫자를 입력하라는 메세지와 함께 메뉴가 다시 출력된다.
 - subtype의 경우 지정된 subtype이외의 숫자가 들어오면 type 선택하는 menu로 돌아가게 된다.