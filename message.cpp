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

Message::Message(const Message& msj) { 

    this->blockCount = msj.blockCount;
    this->isEncrypted = msj.isEncrypted;

    text = new char[strlen(msj.text) + 1];
    strcpy(this->text, msj.text);

    if (blockCount > 0) {
        //am adaugat un safeguard fata de commitul anterior, verific blockcount ul 

        blocks = new BigInt[blockCount];
        memcpy(blocks, msj.blocks, blockCount * sizeof(BigInt));

    } else {
        blocks = nullptr;
    }
       

}

Message& Message::operator=(const Message& msj){

    if (this ==  &msj) return *this;

    delete[] text;
    text = new char[strlen(msj.text) + 1];
    strcpy(text, msj.text);

    delete[] blocks;
    blockCount = msj.blockCount;
    isEncrypted = msj.isEncrypted;

    if (blockCount > 0) {

        blocks = new BigInt[blockCount];

        memcpy(this->blocks, msj.blocks, blockCount* sizeof(BigInt));

        // nu cred ca am zis niciodata in codebase, dar e echivalent cu :
        // for (int i = 0; i < blockCount; i++)
        //     blocks[i] = msj.blocks[i];

    } else {
        blocks = nullptr;
    }


    return *this;

}

void Message::toBlocks( ){


    if (blocks) delete[] blocks; //eliberez inainte de a realoca

    blockCount = strlen(this->getText());
    blocks = new BigInt[blockCount];

    for (int i = 0 ; i<blockCount; i++){

        blocks[i] = BigInt((uint64_t) text[i]);  //castez ca sa se potr pe constructoru de la BIGINT
    }

}

void Message::fromBlocks() { 

        if (text) delete[] text;

        text= new char[blockCount+1];
        

    for (int i = 0 ; i<blockCount; i++){
        text[i] = (char)blocks[i].getLimb(0); //imi ia pe least significant limb 64 biti si mi l casteaza in char
    }

    text[blockCount] = '\0'; 
    isEncrypted = false; //cu metoda asta o sa reconstruiesc text din blocuri decriptate, automat nu vor mai fi criptate in acest punct al programului


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

Message::~Message(){
 if (text) delete[] text;       
 if ( blocks) delete[] blocks;
}