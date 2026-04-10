#include "limitsreader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

std::optional<LimitsReader::Limits> LimitsReader::read(const QString &filename)
{

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Не удалось открыть файл" << filename;
        return std::nullopt;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull())
    {
        qDebug() << "Ошибка парсинга JSON " << filename;
        return std::nullopt;
    }

    QJsonObject rootObj = doc.object();
    if (rootObj.isEmpty()) {
        qDebug() << "Ошибка парсинга JSON: пустой корневой объект";
        return std::nullopt;
    }

    QJsonArray limitsArray = rootObj["limits"].toArray();
    if (limitsArray.isEmpty()) {
        qDebug() << "Ошибка парсинга JSON: массив limits пуст";
        return std::nullopt;
    }

    Limits limits{};

    QJsonObject limitsObj = limitsArray[0].toObject();

    QJsonArray   xArray = limitsObj["X"].toArray();
    if (xArray.size() >= 2)
    {
        limits.xMin = xArray[0].toInt();
        limits.xMax = xArray[1].toInt();
    }
    else
    {
        qDebug() << "Ошибка: массив X должен содержать 2 значения";
        return std::nullopt;
    }

    QJsonArray   yArray = limitsObj["Y"].toArray();
    if (yArray.size() >= 2)
    {
        limits.yMin = yArray[0].toInt();
        limits.yMax = yArray[1].toInt();
    }
    else
    {
        qDebug() << "Ошибка: массив Y должен содержать 2 значения";
        return std::nullopt;
    }

    QJsonArray   vArray = limitsObj["V"].toArray();
    if (vArray.size() >= 2)
    {
        limits.vMin = vArray[0].toInt();
        limits.vMax = vArray[1].toInt();
    }
    else
    {
        qDebug() << "Ошибка: массив V должен содержать 2 значения";
        return std::nullopt;
    }


    QJsonArray   mArray = limitsObj["M"].toArray();
    if (mArray.size() >= 2)
    {
        limits.mMin = mArray[0].toInt();
        limits.mMax = mArray[1].toInt();
    }
    else
    {
        qDebug() << "Ошибка: массив M должен содержать 2 значения";
        return std::nullopt;
    }

    QJsonArray   sArray = limitsObj["S"].toArray();
    if (sArray.size() >= 2)
    {
        limits.sMin = sArray[0].toInt();
        limits.sMax = sArray[1].toInt();
    }
    else
    {
        qDebug() << "Ошибка: массив S должен содержать 2 значения";
        return std::nullopt;
    }

    QJsonArray   aArray = limitsObj["A"].toArray();
    if (aArray.size() >= 2)
    {
        limits.aMin = aArray[0].toDouble();
        limits.aMax = aArray[1].toDouble();
    }
    else
    {
        qDebug() << "Ошибка: массив M должен содержать 2 значения";
        return std::nullopt;
    }

    QJsonArray   pArray = limitsObj["P"].toArray();
    if (pArray.size() >= 2)
    {
        limits.pMin = pArray[0].toInt();
        limits.pMax = pArray[1].toInt();
    }
    else
    {
        qDebug() << "Ошибка: массив P должен содержать 2 значения";
        return std::nullopt;
    }

    return limits;
}

