class Message { 


    private:

    char* text;
    BigInt* blocks;
    int blockCount;
    bool isEncrypted;

    public:

        Message(const char* msj);
        Message(const Message& msj);
        Message& operator=(const Message& msj);

        ~Message();

        void toBlocks();

        void fromBlocks();

       int getBlockCount() const;
       bool getIsEncrypted() const;
       BigInt* getBlocks() const;
       const char* getText() const;

      friend ostream& operator<<(ostream& os, const Message& msj);
      friend istream& operator>>(istream& is,  Message& msj);

};