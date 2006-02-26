   
namespace servlet {    
    template<typename T>
    T HttpSession::getAttribute(const std::string& name) const {
        T value;
        const std::string obj = __getAttributeImpl(name);
        if(obj.empty())
            return value; //or throw an exception?
        {
            std::stringstream stream(obj);
            boost::archive::binary_iarchive ia(stream, boost::archive::no_header);
            ia >> value;
        }
        return value;
    }    
    
    template<typename T>
    void HttpSession::setAttribute(const std::string& name, const T& value){
        std::stringstream stream;
        {
            boost::archive::binary_oarchive oa(stream,boost::archive::no_header);
            oa << value;
        }//call ~oarchive
        std::string str_val = stream.str();
        __setAttributeImpl(name, str_val);
    }

}
