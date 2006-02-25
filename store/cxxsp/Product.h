#ifndef PRODUCT_H
#define PRODUCT_H

#include <boost/serialization/access.hpp>
#include <string>

class Product {
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & product_id;
        ar & category_id;
        ar & name;
        ar & price;
        ar & img;
        ar & desc;
        ar & recomended;
    }    
public:
    Product() 
        : product_id(0), category_id(0), name("Not available"),price(0.0f), 
          img("images/notavail.png"), desc("Not available"), recomended(false)
        { }
    long product_id;
    long category_id;
    std::string name;
    float price;
    std::string img;
    std::string desc;
    bool recomended;
};


#endif//PRODUCT_H

