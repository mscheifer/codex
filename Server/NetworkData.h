#pragma once
#include <string>

struct ChatHandler {
    std::list<std::string> chatHistory;
    std::string chatBuffer;
    sf::Text chatText;
    sf::Font chatFont;
    bool typing;    
    
    ChatHandler();
    void drawChat(sf::RenderWindow & window);
    void sendChatMessage(sf::TcpSocket & socket);
    void receiveChatMessage(const NetworkPacket & netPack );
}
