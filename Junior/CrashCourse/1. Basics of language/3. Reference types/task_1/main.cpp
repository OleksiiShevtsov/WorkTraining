#include <cstdio>
#include <cstddef>
#include <iostream>
#include <cwchar> // for wprintf

struct JBall{
    JBall():m_r{}{}
    JBall(int x, int y, int r):
        m_x{ x }, m_y{ y }, m_r{ r }{}
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }
    void print(){ printf("x: %d, y: %d\n", m_x, m_y); }
private:
    int m_x;
    int m_y;
    const int m_r;
};

struct N{
    char name[256];
};

void print_name(N* n, size_t size){
    for(size_t i = 0; i < size; i++){
        printf("name: %s\n", (n++)->name);
    }
}

void print_name(N& n){
    printf("name: %s\n", n.name);
}

int main()
{
    uint32_t i{};
    uint32_t *i_ptr{ &i };
    printf("i: %d, i_ptr: %p;\n", i, i_ptr);
    *i_ptr = 100;
    printf("*i_ptr: %d, i_ptr: %p;\n", *i_ptr, i_ptr);

    JBall jball{1, 2, 3};
    JBall *jball_ptr{ &jball };
    jball_ptr->setX(10);
    jball_ptr->setY(20);

    printf("jball x: %d, ", jball_ptr->getX());
    printf("jball y: %d\n", (*jball_ptr).getY());

    ////  Pointer and array, link  ///////////////////

    N n[] = {"name1", "name2", "name3"};
    print_name(n, sizeof (n)/sizeof (N));   // pointer
    print_name(n[0]);                       // link

    ////  Buffer overflow  ///////////////////////////

    int m[]{1, 2, 3};
    //m[3] = 4;     //STOP
    //*(m + 3) = 4; //STOP

    ////  connected list  //////////////////////////////////////////////

    struct Element{
        Element* next{ nullptr };
        void insert_after(Element* new_element){
            this->next = new_element;
        }
        char c;
        short oper_num;
    };

    Element elem1, elem2, elem3, elem4;

    elem1.c = 'A';
    elem2.c = 'l';
    elem3.c = 'e';
    elem4.c = 'x';

    elem1.insert_after(&elem2);
    elem2.insert_after(&elem3);
    elem3.insert_after(&elem4);

    for(Element* curNode = &elem1; curNode; curNode = curNode->next){
        printf("%c", curNode->c);
    }
    printf("\n");

    /////  Argument const  ///////////////////////////////////////////////

    auto m_i{ 10 };     //int
    auto m_l{ 10L };    //long
    auto m_f{ 10.0f };  //float
    auto m_d{ 10.0 };   //double
    auto m_b{ false };  //bool
    auto m_s{ "str" };  //char[4]

    auto& ref_i{ m_i };             //int&
    const auto& const_ref_i{ m_i }; //const int&
    auto p_i{ &m_i };               //int*
    auto const_p_i{ &m_i };         //const int*

    JBall arr_jball[10];

    int num = 10;
    for(auto& i : arr_jball){
        i.setX( num += 10 );
        i.setY( num += 20 );
        i.print();
    }

    return 0;
}
