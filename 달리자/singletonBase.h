#pragma once
//inline도 template처럼 헤더파일에 다 들어가 있어야 확인할 수 없는 외부 참조 오류가 안생김
template <typename T> //template를 사용할 경우 컴파일 하는 파일이 컴파일 하는 순간에 모든 정보를 가지고 있어야함
class singletonBase   //-> 헤더파일에 모든 정보를 담거나, 나눠서 코딩한 경우 소스파일도 include로 포함해야한다.
{
protected:
	static T* singleton; //1. 이 싱글톤 베이스 클래스의 인스턴스 T자리에는 singletonBase대신 자식 클래스의 이름이 바로
						 //들어가도록 template 사용 (그래야 자식 클래스의 유일한 인스턴스를 생성할 때 인스턴스의
						 //클래스 타입이 부모인 singletonBase가 아니라 자기 자신이 되도록 할 수 있다.)
						 //2. static 변수이므로 선언만 클래스 내부에서 하고 정의(메모리 할당) 및 초기화는 외부에서 실행

	singletonBase() {}; //외부에서 임의로 생성자에 접근할 수 없도록 protected에다가 정의
	~singletonBase() {};

public:
	static T* getSingleton(void); //이 클래스 인스턴스, 즉 signleton을 만들어 내보내는 함수 (생성은 처음 한번만)
								  //클래스의 static인 변수나 함수는 클래스의 인스턴스가 생성되어 있지 않아도 사용할
								  //수 있다. 이 getSingleton함수는 최초에 인스턴스가 한개도 없을때에도 실행되어
								  //위에 정의된 인스턴스 singleton을 생성해야 하므로 static 함수이어야한다.
								  //한번 singleton이 생성되고 나서는 singleton을 리턴함.
								  //%% 인스턴스가 없는 경우에도 실행되야 하므로 static 함수는 static변수만을 사용해야
								  //한다. 참조:http://yhs1981.tistory.com/entry/C-클래스에서-사용되는-static-멤버-변수-및-함수

	void releaseSingleton(void); //싱글톤 해제 함수
};

template <typename T>
T* singletonBase<T>::singleton = 0; //클래스 안에서 선언된 static 변수의 정의(메모리 할당) 및 초기화
									//클래스 안에서 정의 및 초기화를 하지 않으므로 인스턴스가 생성될때마다 
									//정의 및 초기화를 반복하지 않는다.
template<typename T>
inline T * singletonBase<T>::getSingleton(void)
{
	if (!singleton)
	{
		singleton = new T; //singleton은 T의 포인터. new T로 힙에 T를 동적 할당한 후, 그 주소값을
	}					   //singleton에 리턴함

	return singleton;
}//이 함수를 호출하면 singleton을 return하므로 getSingeton() == singleton 이다.

template<typename T>
inline void singletonBase<T>::releaseSingleton(void)
{
	if (singleton)
	{
		delete singleton;

		singleton = 0;
	}
}