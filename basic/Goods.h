#ifndef _QPT_HEADER_GOODS_INFORMATION_H_
#define _QPT_HEADER_GOODS_INFORMATION_H_

#include <QString>

QT_USE_NAMESPACE

class Goods {
public:
    // barcode & name
    Goods(const QString &strBarcode,
            const QString &strName = QString());
    void SetName(const QString &strName);
    QString GetBarcode() const;
    QString GetName() const;

    // category
    void SetCategory(const QString &strCat,
            const QString &strSubCat = QString());
    QString GetCategory() const;
    QString GetSubCategory() const;

    // quantitative unit
    void SetQuantUnit(const QString &strUnit);
    QString GetQuantUnit() const;

    // unit price
    void SetUnitPrice(float fPrice);
    float GetUnitPrice() const;

private:
    QString m_strBarcode;       // barcode
    QString m_strName;          // name

    QString m_strCategory;      // category
    QString m_strSubCategory;   // subcategory

    QString m_strUnit;          // quantitative unit
    float m_fUnitPrice;         // unit price
};

#endif // Goods.h
