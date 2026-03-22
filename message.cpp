#include <iostream>
#include "bigint.h"
#include "rsakey.h"
#include "rsaengine.h"
#include "message.h"



Message::Message(const char* msj){


    text = new char[strlen(msj) + 1];
    strcpy(text, msj);
    blocks = nullptr; // aici e nul pana cand apelez toBlocks(), imi va initializa cu conversia corecta
    blockCount = 0;
    isEncrypted = false; //aici asum ca initial nu e criptat, destul de straightforward
}



int Message::getBlockCount() const{

        return this->blockCount;
}

bool Message::getIsEncrypted() const {
    return this->isEncrypted;
}

BigInt* Message::getBlocks() const{
    return this->blocks;
}

const char* Message::getText() const{
    return this->text;

}

~Message(){

}