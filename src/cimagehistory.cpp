#include "cimagehistory.h"


CImageHistory::CImageHistory(int ms)
{
    max_size = ms;
    cur = data.begin();
}

QImage CImageHistory::Undo(int steps)
{
    if (data.size() == 0) {
        return QImage();
    }
    //data.back();
    int n = data.size();
    for (; steps > 1 && n > 1; steps--) {
        n--;
        cur++;
    }
    list<QImage>::iterator it = cur;
    cur++;
    if (cur == data.end()) {
        cur = it;
    }

    return *cur;

}

QImage CImageHistory::Current()
{
    return *cur;
}

QImage CImageHistory::Redo(int steps)
{
    if (data.size() == 0) {
        return QImage();
    }
    for (; steps >= 1 && cur != data.begin(); steps--) {
        cur--;
    }
    return *cur;
}

void CImageHistory::Add(QImage& in)
{
    while(data.size() != 0 && cur != data.begin()) {
        data.pop_front();
    }
    if (data.size() >= max_size) {
        data.pop_back();
    }
    data.push_front(in);
    cur = data.begin();
}
