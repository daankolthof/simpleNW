
#if !defined(_HANDLER_H)
#define _HANDLER_H


class Handler {
public:
    virtual void OnConnectionOpen() = 0;
    virtual void OnConnectionClose() = 0;
    virtual void OnReceive() = 0;
    virtual void OnSend() = 0;
};

#endif  //_HANDLER_H
