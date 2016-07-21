#ifndef _QPT_HEADER_ITEM_OFFER_H_
#define _QPT_HEADER_ITEM_OFFER_H_

#include <QString>

#include <QList>
#include <QHash>

QT_USE_NAMESPACE

class Goods;
class Item;

class ItemOffer {
public:
    ItemOffer();
    virtual ~ItemOffer();

    // offer type
    QString GetType() const;

    // offer goods
    void InsertGoods(Goods *pGoods);
    void RemoveGoods(Goods *pGoods);
    QList<Goods *> GetGoods() const;

    // search offer goods
    bool Contains(const QString &strBarcode) const;
    bool Contains(const Goods *pGoods) const;

    // offer
    virtual float GetSaving(const Item *pItem) const = 0;

    // offer info
    QString GetOfferText(const Item *pItem) const;
    QString GetOfferText(const QList<Item *> &pItemList) const;

protected:
    virtual QString GetHeadText() const = 0;
    virtual QString GetItemText(const Item *pItem) const = 0;

protected:
    QString m_strType;  // offer name

    QHash<QString, Goods *> m_pGoodsMap;    // list of offer goods
};

#endif // ItemOffer.h
