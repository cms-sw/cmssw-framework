#include "CondCore/ORA/interface/Database.h"
#include "CondCore/ORA/interface/Container.h"
#include "CondCore/ORA/interface/Transaction.h"
#include "CondCore/ORA/interface/Exception.h"
#include "CondCore/ORA/test/TestBase.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace ora {

  class TestORAInit : public TestBase {
  public:
    TestORAInit(): 
      TestBase( "testORAInit" ){
    }

    virtual ~TestORAInit(){
    }

    void execute( const std::string& connStr ){
      ora::OId id0, id1, id2, id3;
      {
	//clean up if needed
	ora::Database db;
	db.connect( connStr );
	db.transaction().start( false );
	if( db.exists() ) db.drop();
	db.transaction().commit();
	db.disconnect();
      }
      {
	//inserting with automatic database schema creation
	ora::Database db;
	db.configuration().properties().setFlag( ora::Configuration::automaticDatabaseCreation() );
	db.connect( connStr );
	db.transaction().start( false );
	int data = 99;
	id0 = db.insert( data );
	std::cout << "** inserted data contid="<<id0.containerId()<<" oid="<<id0.itemId()<<std::endl;
	std::string s("ORATest1234567890");
	id1 = db.insert( s );
	std::cout << "** inserted data contid="<<id1.containerId()<<" oid="<<id1.itemId()<<std::endl;
	id2 = db.insert( 1000 );
	std::cout << "** inserted data contid="<<id2.containerId()<<" oid="<<id2.itemId()<<std::endl;
	db.flush();
	db.transaction().commit();
      }
      {
	//reading back
	sleep();
	ora::Database db;
	db.connect( connStr );
	db.transaction().start();
	boost::shared_ptr< int > r0 = db.fetch< int >( id0 );
	std::cout << "** read data contid="<<id0.containerId()<<" oid="<<id0.itemId()<<" data="<<*r0<<std::endl;
	boost::shared_ptr< std::string > r1 = db.fetch< std::string >( id1 );
	std::cout << "** read data contid="<<id1.containerId()<<" oid="<<id1.itemId()<<" data="<<*r1<<std::endl;
	db.transaction().commit();
      }
      {
	//clean up
	ora::Database db;
	db.connect( connStr );
	db.transaction().start( false );
	if( db.exists() ) db.drop();
	db.transaction().commit();
	db.disconnect();
      }
      {
	//inserting with automatic container creation: requires explicit database creation, or it throws
	ora::Database db;
	db.configuration().properties().setFlag( ora::Configuration::automaticContainerCreation() );
	db.connect( connStr );
	db.transaction().start( false );
	int data = 99;
	try {
	  id0 = db.insert( data ); 
	} catch ( ora::Exception& e ){
	  std::cout << "# Expected exception:"<<e.what()<<std::endl;
	}
	if( !db.exists() ){
	  std::cout << "# Db does not exists, creating it..."<<std::endl;
	  db.create();
	}
	id0 = db.insert( data ); 
	std::cout << "** inserted data contid="<<id0.containerId()<<" oid="<<id0.itemId()<<std::endl;
	std::string s("ORATest1234567890");
	id1 = db.insert( s );
	std::cout << "** inserted data contid="<<id1.containerId()<<" oid="<<id1.itemId()<<std::endl;
	id2 = db.insert( 1000 );
	std::cout << "** inserted data contid="<<id2.containerId()<<" oid="<<id2.itemId()<<std::endl;
	db.flush();
	db.transaction().commit();
      }
      {
	//reading back
	sleep();
	ora::Database db;
	//db.configuration().setMessageVerbosity( coral::Debug );
	db.connect( connStr );
	db.transaction().start();
	boost::shared_ptr< int > r0 = db.fetch< int >( id0 );
	std::cout << "** read data contid="<<id0.containerId()<<" oid="<<id0.itemId()<<" data="<<*r0<<std::endl;
	boost::shared_ptr< std::string > r1 = db.fetch< std::string >( id1 );
	std::cout << "** read data contid="<<id1.containerId()<<" oid="<<id1.itemId()<<" data="<<*r1<<std::endl;
	db.transaction().commit();
      }
      {
	//clean up
	ora::Database db;
	//db.configuration().setMessageVerbosity( coral::Debug );
	db.connect( connStr );
	db.transaction().start( false );
	if(db.exists()) db.drop();
	db.transaction().commit();
	db.disconnect();
      }
    }
  };
}

int main(int argc, char** argv){
  ora::TestORAInit test;
  test.run();
}

