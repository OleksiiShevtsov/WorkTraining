struct JBall{
    JBall( int x, int y, int r ):
        m_x{ x }, m_y{ y }, m_r{ r }{
        if(m_r < 0){
            throw std::runtime_error{"Radius can not be less than 0"};
        }
        countJball++; }
    ~JBall(){ countJball--; /*throw*/; };
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    void setX( int x ) { m_x = x; }
    void setY( int y ) { m_y = y; }
    // function that does not throw an exception
    void print() noexcept { printf("x: %d, y: %d\n", m_x, m_y); }
    static void printCountJball(){ printf("countJball: %d\n", countJball); }

    static int countJball;
private:
    int m_x;
    int m_y;
    const int m_r;
};

int JBall::countJball{ 0 };
