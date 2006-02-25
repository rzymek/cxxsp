#ifndef CART_ITEM_H
#define CART_ITEM_H

#include <string>
#include "Product.h"

class cart_item {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & quantity;
        ar & product;
    }    
public:
    int quantity;
    Product product;

    bool operator==(int a_quantity) const //for use with boost::lambda
        { return quantity == a_quantity; }
};


#endif//CART_ITEM_H

