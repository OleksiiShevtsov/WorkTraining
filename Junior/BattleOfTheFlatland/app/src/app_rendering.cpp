#include "app/app_rendering.h"

bof::app::AppRendering::AppRendering() :
    m_window{ std::make_shared< sf::RenderWindow >( sf::VideoMode( 
        sf::Vector2u(
            common::GlobalSettings::sizeWindowX, 
            common::GlobalSettings::sizeWindowY ) ),
        "Battle Of The Flatland" ) },
    m_player{ std::make_shared< Player >() }
{
    m_inputHandler = std::make_shared< InputHandler >( m_window );
}

std::shared_ptr< bof::app::Player > bof::app::AppRendering::getPlayer() const
{
    return m_player;
}

std::shared_ptr< bof::app::InputHandler > bof::app::AppRendering::getInputHandler() const
{
    return m_inputHandler;
}

void bof::app::AppRendering::setDynamicObject( const common::ID id, common::Point dot, common::Angle alpha )
{
    m_dynamicObject.push_back( DynamicObject{ id, dot, alpha } );
}

bool bof::app::AppRendering::isWindowOpen() const 
{
    return m_window->isOpen();
}

void bof::app::AppRendering::draw()
{
    sf::Event event;
    if ( m_window->pollEvent( event ) )
    {
        if (event.type == sf::Event::Closed)
            m_window->close();
    }
    m_window->clear( sf::Color( 25, 25, 25 ) );

    drowMap();
    drawDynamicObject();
    m_window->display();

    m_dynamicObject.clear();
}

void bof::app::AppRendering::drowMap()
{

}

void bof::app::AppRendering::drawDynamicObject()
{
    for( const auto &el : m_dynamicObject )
    {
        sf::CircleShape circle( 10.f );
        circle.setFillColor( sf::Color( 120, 20, 20 ) );
        circle.setPosition( sf::Vector2f( el.dot.x, el.dot.y ) );
        m_window->draw( circle );
    }
}
