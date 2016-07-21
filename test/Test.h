#ifndef _QPT_HEADER_ROUTINE_TEST_H_
#define _QPT_HEADER_ROUTINE_TEST_H_

class Goods;
class Item;
class Cart;
class ItemOffer;

extern void PrintGoods(const Goods *pGoods);
extern void PrintItem(const Item *pItem);
extern void PrintCart(const Cart *pCart);
extern void PrintOffer(const ItemOffer *pOffer);

extern void TestItemCart();
extern void TestItemOffer();

extern void TestParser();

extern void TestModule();

extern void TestAll();

#endif // Test.h
