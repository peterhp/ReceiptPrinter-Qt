#ifndef _QPT_HEADER_OFFER_BUY_THREE_FREE_ONE_H_
#define _QPT_HEADER_OFFER_BUY_THREE_FREE_ONE_H_

#include "ItemOffer.h"

#include <QString>

#define OFFER_TYPE_BUY3FREE1    "BUY_THREE_GET_ONE_FREE"

QT_USE_NAMESPACE

class OfferBuy3Free1 : public ItemOffer {
public:
    OfferBuy3Free1();

    float GetSaving(const Item *pItem) const;

private:
    QString GetHeadText() const;
    QString GetItemText(const Item *pItem) const;
};

#endif // OfferBuy3Free1.h
