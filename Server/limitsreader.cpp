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

    auto readRange = [&](const QString& key, int& minVal, int& maxVal) -> bool {
        QJsonArray arr = limitsObj[key].toArray();
        if (arr.size() >= 2) {
            minVal = arr[0].toInt();
            maxVal = arr[1].toInt();
            return true;
        }
        qDebug() << "Ошибка: массив" << key << "должен содержать 2 значения";
        return false;
    };

    if (!readRange("X", limits.xMin, limits.xMax)) return std::nullopt;
    if (!readRange("Y", limits.yMin, limits.yMax)) return std::nullopt;
    if (!readRange("V", limits.vMin, limits.vMax)) return std::nullopt;
    if (!readRange("M", limits.mMin, limits.mMax)) return std::nullopt;
    if (!readRange("S", limits.sMin, limits.sMax)) return std::nullopt;
    if (!readRange("P", limits.pMin, limits.pMax)) return std::nullopt;

    QJsonArray   aArray = limitsObj["A"].toArray();
    if (aArray.size() >= 2)
    {
        limits.aMin = aArray[0].toDouble();
        limits.aMax = aArray[1].toDouble();
    }
    else
    {
        qDebug() << "Ошибка: массив A должен содержать 2 значения";
        return std::nullopt;
    }

    return limits;
}

