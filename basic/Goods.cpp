#include "Goods.h"

#include <QString>

QT_USE_NAMESPACE

Goods::Goods(const QString &strBarcode, const QString &strName) {
    m_strBarcode = strBarcode;
    m_strName = strName;

    m_strCategory = QString();
    m_strSubCategory = QString();

    m_strUnit = QString();
    m_fUnitPrice = 0;
}

void Goods::SetName(const QString &strName) {
    m_strName = strName;
}

QString Goods::GetBarcode() const {
    return m_strBarcode;
}

QString Goods::GetName() const {
    return m_strName;
}

void Goods::SetCategory(const QString &strCat, const QString &strSubCat) {
    m_strCategory = strCat;
    m_strSubCategory = strSubCat;
}

QString Goods::GetCategory() const {
    return m_strCategory;
}

QString Goods::GetSubCategory() const {
    return m_strSubCategory;
}

void Goods::SetQuantUnit(const QString &strUnit) {
    m_strUnit = strUnit;
}

QString Goods::GetQuantUnit() const {
    return m_strUnit;
}

void Goods::SetUnitPrice(float fPrice) {
    m_fUnitPrice = fPrice;
}

float Goods::GetUnitPrice() const {
    return m_fUnitPrice;
}

