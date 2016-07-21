#ifndef _QPT_HEADER_ITEM_CART_H_
#define _QPT_HEADER_ITEM_CART_H_

#include <QString>

#include <QHash>
#include <QList>

QT_USE_NAMESPACE

class Goods;
class ItemOffer;

class Item {
public:
    // goods & goods barcode
    Item(Goods *pGoods);
    QString GetBarcode() const;
    Goods *GetGoods() const;

    // quantity
    void AddGoods(int nCount = 1);
    void DelGoods(int nCount = 1);
    int GetQuant() const;

    // item offer
    void SetOffer(ItemOffer *pOffer);
    ItemOffer *GetOffer() const;

    // price
    float GetOriginalPrice() const;
    float GetSaving() const;
    float GetActualPrice() const;

    // bill text
    QString GetBillText() const;

private:
    Goods *m_pGoods;    // goods info
    int m_nQuant;       // quantity

    ItemOffer *m_pOffer; // offer for the goods
};


class Cart {
public:
    Cart();
    // clear items
    ~Cart();

    // goods list
    void Insert(Goods *pGoods, int nCount = 1);
    void Remove(Goods *pGoods, int nCount = 1);
    QList<Item *> GetItems() const;
    void ClearItems();

    // search goods
    bool Contains(const QString &strBarcode) const;
    bool Contains(const Goods *pGoods) const;
    Item *Find(const QString &strBarcode) const;
    Item *Find(const Goods *pGoods) const;

    // offer
    void SetOffer(ItemOffer *pOffer);

    // cost & saving
    float GetOriginalCost() const;
    float GetActualCost() const;
    float GetSaving() const;

    // bill text
    QString GetBillText() const;

private:
    // items list stored in hash map for fast searching
    QHash<QString, Item *> m_pItemMap;
};

#endif // ItemCart.h
