#include "CondCore/ORA/interface/Record.h"
#include "AnyData.h"
#include "RecordDetails.h"

#include<map>

namespace {
  ora::AllKnowTypeHandlers  allKnowTypeHandlers;
}


namespace ora {
 struct RecordSpecImpl {

    struct Item {
      Item(std::string const & iname,
           TypeHandler const * ih) : name(iname), handler(ih){}
      std::string name;
      TypeHandler const * handler;
    };


    std::vector<Item> items;

    typedef std::map<std::string, int> Lookup;
    
    Lookup indexes;

   
  };


  RecordSpec::RecordSpec() : specs(new RecordSpecImpl) {}

  RecordSpec::~RecordSpec(){}

  size_t RecordSpec::add(std::string const & name, std::type_info const & type) {
    // check if already exists
    TypeHandler const * th =  allKnowTypeHandlers(type);
    // check if 0...
    specs->items.push_back(RecordSpecImpl::Item(name,th));
    specs->indexes.insert(std::make_pair(specs->items.back().name,(int)(specs->items.size())-1));
    return specs->items.size()-1;
  }



  Record::Record(){}
    
  Record::Record(RecordSpec ispecs) {
    init(ispecs);
  }
    
  void Record::init(RecordSpec ispecs) {
    destroy();
    specs = ispecs.specs;
    size_t s = specs->items.size();
    m_field.resize(s);
    for (size_t i=0; i<s; ++i) {
      RecordSpecImpl::Item const & item = specs->items[i];
      item.handler->create(m_field[i]);
    }
  }

  Record::~Record() {
    destroy();
  }

  void Record::destroy() {
    if (m_field.empty()) return;
    for (size_t i=0; i<m_field.size(); ++i) {
      RecordSpecImpl::Item const & item = specs->items[i];
      item.handler->destroy(m_field[i]);
    }
    m_field.clear();
  }


  void Record::swap(Record & lh) {
    specs.swap(lh.specs);
    m_field.swap(lh.m_field);
  }


  size_t Record::size() const { 
    return m_field.size();
  }

  int Record::index(std::string const & iname) const {
    if (m_field.empty()) return -1;
    RecordSpecImpl::Lookup::const_iterator p = specs->indexes.find(iname);
    return (p==specs->indexes.end()) ? -1 : (*p).second;
  }


  std::type_info const * Record::type(int i) const {
    return specs->items[i].handler->type;
  }

  void const * Record::address(int i) const {
    return specs->items[i].handler->address(m_field[i]);
  }

  void const * Record::get(int i) const {
    return specs->items[i].handler->get(m_field[i]);
  }

  void Record::set(int i, void * p) {
    specs->items[i].handler->set(m_field[i],p);
  }


  std::string const & Record::name(int i) const {
    return specs->items[i].name;
  }
 


}
