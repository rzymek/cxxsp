#define EXPORT_SERVLET(servlet)                 \
    extern "C" servlet::HttpServlet* create()   \
        { return new servlet(); }
