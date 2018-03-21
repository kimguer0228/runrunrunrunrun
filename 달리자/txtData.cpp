#include "stdafx.h"
#include "txtData.h"

HRESULT txtData::init()
{
	return S_OK;
}

void txtData::release()
{
}

void txtData::txtSave(const char * saveFileName, vector<string> vStr)
{
	HANDLE file; //실제 텍스트 파일의 주소를 저장할 보이드 포인터

	char str[128];
	DWORD write;

	strncpy_s(str, 128, vectorArrayCombine(vStr), 126); //str에 vStr을 집어넣기
					//vectorArrayCombine는 vStr에 담긴 4개의 데이터를 한개의 문자열로 바꿔줌
	file = CreateFile(saveFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //CreateFile는 텍스트파일 bmp등 다양한 파일 만들 수있음
	//두번째 인자는 file에 저장된 주소를 이용해 무슨일을 할지 알려주는 부분								 //없으면 만들어서(CREATE_ALWAYS) 주소를 file에 리턴, 있으면 기존파일 주소를 리턴.

	WriteFile(file, str, 128, &write, NULL); //str의 내용을 file에 적용 이미 파일이 있으면 덮어씌우기함

	CloseHandle(file); //핸들 메모리 해제
}

char * txtData::vectorArrayCombine(vector<string> vArray)
{
	char str[128];

	ZeroMemory(str, sizeof(str)); //ZeorMemory는 memset 함수로 시작점 주소, 지정된 길이를 받아 그 길이만큼을 초기화해줌

	for (int i = 0; i < vArray.size(); ++i) //i++?
	{
		strncat_s(str, 128, vArray[i].c_str(), 126); //strncpy_s는 전체를 덮어씌우고, strncat_s은 기존 데이터에 이어서 입력
		
		if (i + 1 < vArray.size()) //i+1이 vArray.size()이면 벡터의 마지막칸 => 마지막칸을 제외한 값들
		{
			strcat_s(str, ","); //마지막칸 아니면 , 추가 //strcat쓰면 C4996에러 나와서 임시로 _s추가
		}
	}

	return str;
}

vector<string> txtData::txtLoad(const char * loadFileName)
{
	HANDLE file;

	char str[128];
	DWORD read;

	file = CreateFile(loadFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
								//save때와는 다르게 READ가 주목적임을 알린다
	ReadFile(file, str, 128, &read, NULL);

	CloseHandle(file); //핸들 메모리 해제

	return charArraySeparation(str);
}

vector<string> txtData::charArraySeparation(char charArray[])
{
	vector<string> vArray; //문자열을 쪼개서 저장할 벡터

	char* separator = ",";
	char* token;

	//%% strtok C4996 에러 => strtok_s로 변경
	//%% strtok_s는 strtok의 두 인자에 더해 잘린부분을 저장할 컨테이너 주소를 인자로 받는다.
	char* temp = NULL; //저장할곳

	token = strtok_s(charArray, separator, &temp);	//strtok는 separator 나오기전까지 문자열을 내보내고 함수를 나옴
	vArray.push_back(token);						//한번 실행되면 separator 뒤를 strtok 내부에? 저장 (=> 이미 읽은 부분은 잘라내준다)
													//-> 한번 실행 후에는 strtok의 첫번째 인자에는 NULL만 넣어주면 된다

	while (NULL != (token = strtok_s(NULL, separator, &temp))) //분할이 끝나면 strtok은 NULL을 반환
	{
		vArray.push_back(token);
	}

	return vArray;
}

txtData::txtData()
{
}

txtData::~txtData()
{
}
