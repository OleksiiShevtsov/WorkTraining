#include "app/app_rendering.h"

flower::app::AppRendering::AppRendering() :
    m_textNumberOfCoins{ "0" },
    m_textInStatisticsBox{ "press any key to start" },
    m_window{ std::make_shared< sf::RenderWindow >( sf::VideoMode(
        sf::Vector2u( common::GlobalSettings::pixelCountX * common::GlobalSettings::sizePixel,
                      common::GlobalSettings::pixelCountY * common::GlobalSettings::sizePixel + statisticsBoxHeight ) ), "Flower" ) }{
    m_buttonHandler = std::make_shared< ButtonHandler >( m_window );
}

std::shared_ptr< flower::app::ButtonHandler > flower::app::AppRendering::getButtonHandler() const {
    return m_buttonHandler;
}

void flower::app::AppRendering::setSnakeObjects( const common::ID id, const std::vector< common::Point > snake ){
    m_snakeObjects.push_back( ObjSnake{ id, snake } );
}

void flower::app::AppRendering::setFlowerObjects( const common::ID id, const common::Point flower ){
    m_flowerObjects.push_back( ObjFlower{ id, flower } );
}

void flower::app::AppRendering::setTextNumberOfCoins( const common::Coin coin ){
    m_textNumberOfCoins = std::to_string( coin );
}

void flower::app::AppRendering::setTextInStatisticsBox( const std::string text ){
    m_textInStatisticsBox = text;
}

bool flower::app::AppRendering::isWindowOpen() const {
    return m_window->isOpen();
}

void flower::app::AppRendering::drawObjects(){
    sf::Event event;
    while ( m_window->pollEvent( event ) )
    {
        if (event.type == sf::Event::Closed)
            m_window->close();
    }
    m_window->clear( sf::Color( 25, 25, 25 ) );
    drowFlower();
    drowSnake();
    drowMap();
    m_window->display();

    m_snakeObjects.clear();
    m_flowerObjects.clear();
}

void flower::app::AppRendering::drowSnake(){
    for ( auto &snake : m_snakeObjects ){
        for ( size_t i{ snake.items.size() }; i > 0; i-- ){

            if( i == 1 ){
                sf::RectangleShape rectangle;
                rectangle.setSize( sf::Vector2f( common::GlobalSettings::sizePixel, common::GlobalSettings::sizePixel ) );
                rectangle.setFillColor( sf::Color( 200, 200, 100 ) );
                rectangle.move( sf::Vector2f( snake.items[ i - 1 ].x * common::GlobalSettings::sizePixel, snake.items[ i - 1 ].y * common::GlobalSettings::sizePixel ) );
                m_window->draw( rectangle );
                continue;
            }

            sf::RectangleShape rectangle;
            rectangle.setSize( sf::Vector2f( common::GlobalSettings::sizePixel, common::GlobalSettings::sizePixel ) );
            rectangle.setFillColor( sf::Color( 100, 200, 100 ) );
            rectangle.move( sf::Vector2f( snake.items[ i - 1 ].x * common::GlobalSettings::sizePixel, snake.items[ i - 1 ].y * common::GlobalSettings::sizePixel ) );
            m_window->draw( rectangle );
        }
    }
}

void flower::app::AppRendering::drowFlower(){
    for ( auto &flower : m_flowerObjects ){
        sf::RectangleShape rectangle;
        rectangle.setSize( sf::Vector2f( common::GlobalSettings::sizePixel, common::GlobalSettings::sizePixel ) );
        rectangle.setFillColor( sf::Color( 200, 40, 40 ) );
        rectangle.move( sf::Vector2f( flower.item.x * common::GlobalSettings::sizePixel, flower.item.y * common::GlobalSettings::sizePixel ) );
        m_window->draw( rectangle );
    }
}

void flower::app::AppRendering::drowMap(){
    for( std::size_t i{}; i < common::GlobalSettings::pixelCountX; i++ ){
        sf::RectangleShape lineOnX;
        lineOnX.setSize( sf::Vector2f( 1, common::GlobalSettings::pixelCountY * common::GlobalSettings::sizePixel ) );
        lineOnX.setFillColor( sf::Color( 50, 50, 50 ) );
        lineOnX.move(sf::Vector2f( common::GlobalSettings::sizePixel * i, 0 ) );
        m_window->draw( lineOnX );
    }
    for( std::size_t i{}; i < common::GlobalSettings::pixelCountY; i++ ){
        sf::RectangleShape lineOnY;
        lineOnY.setSize( sf::Vector2f( common::GlobalSettings::pixelCountX * common::GlobalSettings::sizePixel, 1 ) );
        lineOnY.setFillColor(sf::Color( 50, 50, 50 ));
        lineOnY.move( sf::Vector2f( 0, common::GlobalSettings::sizePixel * i ) );
        m_window->draw( lineOnY );
    }

    sf::RectangleShape statisticsBoxFrame;
    statisticsBoxFrame.setSize( sf::Vector2f( common::GlobalSettings::pixelCountX * common::GlobalSettings::sizePixel, statisticsBoxHeight ) );
    statisticsBoxFrame.setFillColor(sf::Color( 50, 50, 50 ));
    statisticsBoxFrame.move( sf::Vector2f( 0, common::GlobalSettings::pixelCountY * common::GlobalSettings::sizePixel ) );
    m_window->draw( statisticsBoxFrame );

    sf::RectangleShape statisticsField;
    statisticsField.setSize( sf::Vector2f( common::GlobalSettings::pixelCountX * common::GlobalSettings::sizePixel - statisticsBoxBorderThickness * 2, statisticsBoxHeight - statisticsBoxBorderThickness * 2 ) );
    statisticsField.setFillColor(sf::Color( 25, 25, 25 ));
    statisticsField.move( sf::Vector2f( statisticsBoxBorderThickness, common::GlobalSettings::pixelCountY * common::GlobalSettings::sizePixel + statisticsBoxBorderThickness ) );
    m_window->draw( statisticsField );

    sf::Font font;

    // the path needs to be changed depending on the location of the build
    if ( !font.loadFromFile( "arial.ttf" ) )
    {

        std::cout << "textures not loaded" << std::endl;
    }

    sf::Text textNumberOfCoins( "Coins: " + m_textNumberOfCoins, font, 40 );
    textNumberOfCoins.setPosition( sf::Vector2f( statisticsBoxBorderThickness * 2, common::GlobalSettings::pixelCountY * common::GlobalSettings::sizePixel + statisticsBoxBorderThickness * 2 ) );
    textNumberOfCoins.setFillColor( sf::Color( 50, 200, 50 ) );
    textNumberOfCoins.setStyle( sf::Text::Bold );
    m_window->draw( textNumberOfCoins );

    sf::Text textInStatisticsBox( m_textInStatisticsBox, font, 40 );
    textInStatisticsBox.setPosition( sf::Vector2f( common::GlobalSettings::pixelCountX * common::GlobalSettings::sizePixel / 2, common::GlobalSettings::pixelCountY * common::GlobalSettings::sizePixel + statisticsBoxBorderThickness * 2 ) );
    textInStatisticsBox.setFillColor( sf::Color( 50, 200, 50 ) );
    textInStatisticsBox.setStyle( sf::Text::Bold );
    m_window->draw( textInStatisticsBox );
}
