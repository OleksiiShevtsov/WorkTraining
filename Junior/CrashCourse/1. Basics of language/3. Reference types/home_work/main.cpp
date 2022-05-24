#include <cstdio>
#include <string.h>

struct String{

    String( char* str ){
        for(int i = 0; i < size; i++){
            m_str[i] = *(str + i);
        }
    }

    void read_from(){
        printf("%s\n", m_str);
    }

    void write_to( char symbol, int position ){
        if(position > size){ return; }
        *(m_str + position - 1) = symbol;
    }

    static const size_t size = 10;
    char m_str[ size ];
};

struct Element{
    Element* next{ nullptr };
    Element* prev{ nullptr };

    void insert_after(Element* new_element){
        this->next = new_element;
        next->prev = this;
    }

    void insert_before(Element* new_element){
        this->prev = new_element;
        prev->next = this;
    }

    short num;
};

int main()
{
    String str("text");
    str.read_from();
    str.write_to('T', 1);
    str.read_from();

    ///////////////////////////////////////////////

    Element el1, el2, el3, el4, el5;

    el3.insert_after(&el4);
    el4.insert_after(&el5);

    el3.insert_before(&el2);
    el2.insert_before(&el1);

    int i = 1;
    for(Element* cur = &el1; cur; cur = cur->next){
        cur->num = i++;
    }

    for(Element* cur = &el5; cur; cur = cur->prev){
        printf("num: %d\n", cur->num);
    }

    ///////////////////////////////////////////////

    auto num{ 100 };
    auto *num_ptr{ &num };
    printf("i: %d, i_ptr: %p;\n", num, num_ptr);

    return 0;
}
