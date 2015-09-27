#include "ctest.h"
#include <QImage>
#include <QTime>

#include <QDebug>
#include <QBuffer>
#include <math.h>
#include "cparamhelper.h"
#include "ctesttransform.h"
#include "util.h"
#include "ECC/rsccode.h"

void CTest::Test(CAlgorithm* alg, QString& params, QString& alg_params, QByteArray& result, const QString& container_name, const QString& watermark_name)
{
    QImage in;                                                  //Load test image
    in.load(container_name);
    QImage orig(in);                                            //Save test image to restore it later

    QByteArray bytes;                                           //Input/output arrays
    QByteArray bytes_out;
    QBitArray bits, bits_out, tmp_bits_out;

    srand(QTime::currentTime().msec());                         //Initialize random number generator

    int count = 0;                                              //Defaults
    int bcount = 0;
    tsize = size = block = parity = 0;
    num = 10;

    CTest::SetParams(params);                                   //Parse and set test parameters

    if(block && parity) {                                       //If error correction codes are used
        int encoded_len = ((size + block - 1) / block) * (block + parity);
        SetParams(params);
        QByteArray tmp_bytes;
        QByteArray tmp_bytes_out;
        bytes.resize(size);
        tmp_bytes.resize(encoded_len);
        tmp_bytes_out.resize(encoded_len);
        bytes_out.resize(size);
        RscCode r(parity);
        r.setBlockSize(block);
        r.initialize_ecc();
        for (int i=0; i<num; i++) {
            for (int i=0; i<size; i++) {
                bytes[i] = rand() % 256;
            }
            QBitArray tmpbits = alg->byteToBit(bytes);
            QBitArray tmpbits3(8*size);
            QBitArray tmpbits4(8*encoded_len);
            QByteArray b(8*size, 0);
            for (int i=0; i<tmpbits.size(); i++) {
                b[i] = tmpbits[i] ? 0 : 255;
            }
            QByteArray b2(8*encoded_len, 0);
            r.encode((unsigned char*)b.data(), size*8, (unsigned char*)b2.data());
            QBitArray tmpbits2(8*encoded_len);
            for (int i=0; i<b2.size(); i++) {
                tmpbits2[i] = (b2[i] == (char)255);
            }
            tmp_bytes = alg->bitToByte(tmpbits2);
            alg->SetParams(alg_params);
            alg->GenKey(tmp_bytes);
            alg->Hide(in, tmp_bytes);
            Attack(in, params);
            alg->Restore(in, tmp_bytes_out);

            tmpbits4 = alg->byteToBit(tmp_bytes_out);
            for (int i=0; i<tmpbits4.size(); i++) {
                b2[i] = tmpbits4[i] ? 0 : 255;
            }

            int pos = 0;
            r.decode((unsigned char*)b2.data(), size*8, (unsigned char*)b.data());
            for (int i=0; i<b.size(); i++) {
                tmpbits3[i] = (b[i] == (char) 255);
            }
            bytes_out = alg->bitToByte(tmpbits3);

            for (int i=0; i< bytes.size(); i++) {
                if (bytes[i] == bytes_out[i]) {
                    bcount++;
                }
                if (bytes_out[i] == (char) 255) {
                    pos++;
                }
            }

            bits = alg->byteToBit(bytes);
            tmp_bits_out = alg->byteToBit(bytes_out);

            bits_out.resize(size*8);

            for (int i=0; i<size*8; i++) {
                int v = 0;
                if (tmp_bits_out[i]) {
                    v++;
                    pos++;
                }
                else {
                    v--;
                }
                if (v > 0) {
                    bits_out[i] = true;

                }
                else {
                    bits_out[i] = false;
                }
            }

            for (int i=0; i< bits_out.size(); i++) {
                if (bits[i] == bits_out[i]) {
                    count++;
                }
            }
            in = orig;
        }
        sResults res;
        res.ByER = ((double)count) / bytes.size() / num;
        res.BER = ((double)count) / (size*8) / num;
        result = SERIALIZE(res);
        return;
    }
                                                                        //Case without correction codes
    if (!size) {                                                        //Default watermark size
        size = (in.width() / 8) * (in.height() / 8) / 8;
    }

    if (!tsize) {                                                       //Default embedded amount
        tsize = size;
    }

    int rep = tsize / size;                                             //Number of wm repetitions
    bytes.resize(size * rep);                                           //Resize wm array

    SetParams(params);                                                  //Set class specific params

    for (int i=0; i<num; i++) {                                         //Repeat the embedding N times to take average result then
        for (int i=0; i<size; i++) {                                    //Generate watermark and fill the array with it
            bytes[i] = rand() % 256;// - bytes[i];
            for (int j=1; j<rep; j++) {
                bytes[i+size*j] = bytes[i];
            }
        }

        alg->SetParams(alg_params);                                     //Set embedding algorithm params
        alg->GenKey(bytes);                                             //Gen embedding key
        alg->Hide(in, bytes);                                           //Embed the wm
        Attack(in, params);                                             //Attack the wm (core action)
        alg->Restore(in, bytes_out);                                    //Restore the wm from attacked container

        int pos = 0;
        for (int i=0; i< bytes.size(); i++) {                           //Get number of successfully restored bytes
            if (bytes[i] == bytes_out[i]) {
                bcount++;
            }
            if (bytes_out[i] == (char) 255) {
                pos++;
            }
        }

        bits = alg->byteToBit(bytes);                                   //Convert original and restored watermark to bit arrays
        tmp_bits_out = alg->byteToBit(bytes_out);

        bits_out.resize(size*8);

        for (int i=0; i<size*8; i++) {                                  //Get number of successfully restored bits
            int v = 0;
            for (int j=0; j<rep; j++) {                                 //Take average value among all repeated bits
                if (tmp_bits_out[size*8*j+i]) {
                    v++;
                    pos++;
                }
                else {
                    v--;
                }
            }
            if (v > 0) {
                bits_out[i] = true;

            }
            else {
                bits_out[i] = false;
            }
        }

        for (int i=0; i< bits_out.size(); i++) {                        //Get number of successfully restored bits
            if (bits[i] == bits_out[i]) {
                count++;
            }
        }

        in = orig;                                                      //Restore test image before yet one iteration
    }
    sResults res;
    res.ByER = ((double)count) / bytes.size() / num;
    res.BER = ((double)count) / (size*8) / num;
    result = SERIALIZE(res);                                            //Serialize and return the result (mainly Bit Error Rate)
}

CTest::~CTest() {
}

//Embeds certain or random watermark and saves the result to a file for visual investigation of the introduced distortions
void CTest::Visual(CAlgorithm* alg, QString& params, QString& alg_params, const QString& container_name, const QString* watermark_name, int wm_size)
{
    QImage in;
    CParamHelper ph;
    QString res_name;
    QByteArray bytes;

    ph.AddToMap(&res_name, ph.STRING, "name");                          //Set variables from param string
    ph.AddToMap(&size, ph.INT, "size");
    ph.AddToMap(&tsize, ph.INT, "tsize");
    ph.SetParams(params);

    in.load(container_name);                                            //Load watermark from specified file

    if (watermark_name != 0) {                                          //Either read wm from file
        QFile wm(*watermark_name);
        wm.open(QIODevice::ReadOnly);
        bytes = wm.readAll();
        wm.close();
    }
    else {                                                              //Or generate a random one
        srand(QTime::currentTime().msec());
        bytes.resize(wm_size);
        for (int i=0; i<wm_size; i++) {
            bytes[i] = rand() % 256;
        }
    }

    alg->SetParams(alg_params);                                         //Set embedding algorithm params
    alg->GenKey(bytes);                                                 //Gen embedding key
    alg->Hide(in, bytes);                                               //Embedd the wm
    in.save(res_name);                                                  //Save the container with embedded wm to the file
}

//Reads wm to a bytearray from file containing 1-bit image
QByteArray CTest::LoadPictureWm(QString& name, int& width, int& height)
{
    QImage wm;
    wm.load(name);
    width = wm.width();
    height = wm.height();
    QBitArray bits(wm.width() * wm.height());

    for (int i=0; i<wm.width(); i++) {
        for (int j=0; j<wm.height(); j++) {
            unsigned int p = BLUE(wm.pixel(i, j));
            if (!p) {
                bits[i*wm.height() + j] = 0;
            }
            else {
                bits[i*wm.height() + j] = 1;
            }
        }
    }

    return CAlgorithm::bitToByte(bits);
}

//Saves wm to file as 1-bit image
void CTest::SavePictureWm(QString& name, int width, int height, QVector<double>& data)
{
    QImage wm(width, height, QImage::Format_RGB32);

    for (int i=0; i<wm.width(); i++) {
        for (int j=0; j<wm.height(); j++) {
            int k = PIXELRANGE(data[i*wm.height() + j] * 255);
            wm.setPixel(i, j, PIXEL(k, k, k));
        }
    }

    wm.save(name);
}

//Embeds watermark and saves modified container and watermark into files
void CTest::Save(CAlgorithm* alg, QString& params, QString& alg_params, const QString& container_name)
{
    QString res_name, info, wm_name;
    int type, width, height;
    QByteArray bytes;
    QImage in;

    CParamHelper ph;                                            //Set variables from param string
    ph.AddToMap(&res_name, ph.STRING, "name");
    ph.AddToMap(&info, ph.STRING, "info");
    ph.AddToMap(&size, ph.INT, "size");
    ph.AddToMap(&tsize, ph.INT, "tsize");
    ph.AddToMap(&wm_name, ph.STRING, "wm_name");
    ph.SetParams(params);

    in.load(container_name);                                    //Load test image

    if (wm_name == "") {                                        //If no wm file provided, generate it
        srand(QTime::currentTime().msec());
        bytes.resize(tsize);
        int rep = tsize / size;
        for (int i=0; i<size; i++) {
            bytes[i] = rand() % 256;
            for (int j=1; j<rep; j++) {
                bytes[i+size*j] = bytes[i];
            }
        }
        type = 1;
    }
    else {                                                      //Or load it as a picture
        bytes = LoadPictureWm(wm_name, width, height);
        size = bytes.length();
        tsize = size;
        type = 2;
    }

    alg->SetParams(alg_params);                                 //Hide the wm
    alg->GenKey(bytes);
    alg->Hide(in, bytes);
    QByteArray key = alg->ExportKey();

    in.save(res_name);                                          //Save container to the file

    QFile file(info);                                           //Save watermark to separate file
    file.open(QIODevice::WriteOnly);
    file.write((char*)&type, sizeof(int));

    if (type == 1) {
        int keylen = key.length();
        file.write((char*)&keylen, sizeof(int));
        file.write(key);
        file.write((char*)&tsize, sizeof(int));
        file.write(bytes);
        file.write((char*)&size, sizeof(int));
    } else if (type == 2) {
        file.write((char*)&width, sizeof(int));
        file.write((char*)&height, sizeof(int));
        int keylen = key.length();
        file.write((char*)&keylen, sizeof(int));
        file.write(key);
        file.write(bytes);
    }
    file.close();
}

//Reads watermark and modified container from files and checks whether the wm is embedded in the container
double CTest::Check(CAlgorithm* alg, QString& params, QString& alg_params, const QString& container_name)
{
    QString res_name, info, wm_name;
    int type, width, height, keylen;
    CParamHelper ph;
    QImage in;
    QByteArray bytes, bytes_out;
    QByteArray key;

    ph.AddToMap(&res_name, ph.STRING, "name");                          //Set variables from param string
    ph.AddToMap(&info, ph.STRING, "info");
    ph.AddToMap(&wm_name, ph.STRING, "wm_name");
    ph.SetParams(params);

    in.load(res_name);                                                  //Load container with embedded wm

    QFile file(info);                                                   //Load wm
    file.open(QIODevice::ReadOnly);
    file.read((char*)&type, sizeof(int));
    if (wm_name == "") {
        type = 1;
    }
    else {
        type = 2;
    }
    if (type == 1) {
        file.read((char*)&keylen, sizeof(int));
        key = file.read(keylen);
        file.read((char*)&tsize, sizeof(int));
        bytes.resize(size);
        bytes = file.read(tsize);
        file.read((char*)&size, sizeof(int));
    } else if (type == 2) {
        file.read((char*)&width, sizeof(int));
        file.read((char*)&height, sizeof(int));
        int keylen;
        file.read((char*)&keylen, sizeof(int));
        key = file.read(keylen);
        bytes = file.read(width * height);
    }
    file.close();

    alg->SetParams(alg_params);                                         //Set algorithm params

    if (type == 1) {                                                    //If wm is not a picture
        alg->ImportKey(key);
        alg->Restore(in, bytes_out);                                    //Extract the wm

        QBitArray bits = alg->byteToBit(bytes);                         //Convert bytes to bits for more convenient analysis
        QBitArray bits_out = alg->byteToBit(bytes_out);

        int rep = tsize / size;

        double good;
        for (int i=0; i < size*8; i++) {                                //Find average bit value among all instances of extracted wm
            int v = 0;
            for (int j=0; j<rep; j++) {
                if (bits_out[size*8*j+i]) {
                    v++;
                }
                else {
                    v--;
                }
            }
            if (v > 0 && bits[i] || v <= 0 && !bits[i]) {
                good++;
            }
        }
        return good / (size * 8);                                       //Return percent of properly recognized bits
    } else if (type == 2) {                                             //If wm is a picture
        QVector<double> res(width * height);

        alg->ImportKey(key);                                            //Extract wm
        alg->Restore(in, res);

        SavePictureWm(wm_name, width, height, res);                     //Save the wm as picture
    }
}

//Tries to embed and extract 1-bit picture watermark, saves result as a picture (24-bit white and black picture emulates 1-bit picture for convenience)
void CTest::Picture(CAlgorithm* alg, QString& params, QString& alg_params, const QString& container_name, const QString* watermark_name)
{
    QString res_name;
    QString wm_name;

    CParamHelper ph;                                                //Set variables from param string
    ph.AddToMap(&res_name, ph.STRING, "name");
    ph.AddToMap(&wm_name, ph.STRING, "wm_name");
    ph.AddToMap(&size, ph.INT, "size");
    ph.AddToMap(&tsize, ph.INT, "tsize");
    ph.SetParams(params);

    QImage in;                                                      //Load test image
    in.load(container_name);

    QImage wm;                                                      //Load watermark picture (each watermark bit is stored as RGB byte)
    wm.load(wm_name);

    CTest* test = new CTestTransform;                               //Initialize new test object

    QBitArray bits(wm.width()*wm.height());                         //Input bits
    QByteArray bytes;                                               //Input bytes
    QVector<double> res(wm.width()*wm.height());                    //Result bits

    for (int i=0; i<wm.width(); i++) {                              //Use blue component of each watermark picture byte as watermark bit
        for (int j=0; j<wm.height(); j++) {
            unsigned int p = BLUE(wm.pixel(i, j));
            if (!p) {
                bits[i*wm.height() + j] = 0;
            }
            else {
                bits[i*wm.height() + j] = 1;
            }
        }
    }

    bytes = CAlgorithm::bitToByte(bits);                            //Convert bytes to bits

    QImage resImage(wm);                                            //Open output image file

    alg->SetParams(alg_params);                                     //Embed, attack and extract watermark
    alg->GenKey(bytes);
    alg->Hide(in, bytes);
    test->Attack(in, params);
    alg->Restore(in, res);

    for (int i=0; i<wm.width(); i++) {                              //Convert each bit to black or white pixel of 24-bit RGB picture
        for (int j=0; j<wm.height(); j++) {
            int k = PIXELRANGE(res[i*wm.height() + j] * 255);
            resImage.setPixel(i, j, PIXEL(k, k, k));
        }
    }

    resImage.save(res_name);                                        //Save the result

    delete test;                                                    //Free used memory
}

//Set general test params common for all tests
void CTest::SetParams(QString params)
{
    this->params = params;
    CParamHelper ph;
    ph.AddToMap(&size, ph.INT, "size");
    ph.AddToMap(&tsize, ph.INT, "tsize");
    ph.AddToMap(&block, ph.INT, "block");
    ph.AddToMap(&parity, ph.INT, "parity");
    ph.AddToMap(&num, ph.INT, "num");
    ph.SetParams(params);
}

//Abstract method performing some distortion
void CTest::Attack(QImage& img, QString& params) {}
