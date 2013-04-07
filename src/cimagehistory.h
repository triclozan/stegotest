#ifndef CIMAGEHISTORY_H
#define CIMAGEHISTORY_H
#include <list>
using namespace std;
#include <QImage>
class CImageHistory
{
public:
    list<QImage> data;
    int max_size;
    QImage Undo(int steps = 1);
    QImage Current();
    QImage Redo(int steps = 1);
    void Add(QImage& in);
    CImageHistory(int ms = 10);
    list<QImage>::iterator cur;
};

#endif // CIMAGEHISTORY_H
