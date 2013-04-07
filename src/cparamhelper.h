#ifndef CPARAMHELPER_H
#define CPARAMHELPER_H
#include <QList>
#include <QString>

class CParamHelper
{
public:
    CParamHelper();
    enum paramTypes {INT=0, DOUBLE=1, STRING=2, CHANNEL, TRANSFORM};
    struct sParam {
        void* ptr;
        int type;
        QString name;
        sParam(void* p, int t, QString n) : ptr(p), type(t), name(n) {}
    };
    void AddToMap(void* ptr, int type, QString name);
    void SetParams(QString& params, QString param_sep);
    void SetParams(QString& params);
    void Process(void* ptr, int type, QString value);
    void Clear();
private:
    QList<sParam> paramMap;
};

#endif // CPARAMHELPER_H
