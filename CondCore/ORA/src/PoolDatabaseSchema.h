#ifndef INCLUDE_ORA_POOLDATABASESCHEMA_H
#define INCLUDE_ORA_POOLDATABASESCHEMA_H

#include "IDatabaseSchema.h"
// externals
#include "CoralBase/AttributeList.h"

namespace ora {

    struct PoolDbCacheData {
      PoolDbCacheData();
        
      PoolDbCacheData( int id, const std::string& name, const std::string& className,
                       const std::string& mappingVersion, unsigned int nobjWritten );

      ~PoolDbCacheData();

      PoolDbCacheData( const PoolDbCacheData& rhs );

      PoolDbCacheData& operator=( const PoolDbCacheData& rhs );

      int m_id;
      std::string m_name;
      std::string m_className;
      std::string m_mappingVersion;
      unsigned int m_nobjWr;
  };
  
  class PoolDbCache {
    public:
    PoolDbCache();
    ~PoolDbCache();
    void add( int id, const PoolDbCacheData& data );
    const std::string& nameById( int id );
    int idByName( const std::string& name );
    PoolDbCacheData& find( int id );
    void remove( int id );
    std::map<std::string,PoolDbCacheData* >& sequences();
    void clear();
    
    private:
    PoolDbCacheData m_databaseData;
    PoolDbCacheData m_mappingData;
    std::map<int,PoolDbCacheData > m_idMap;
    std::map<std::string,PoolDbCacheData* > m_sequences;
  };

  class PoolMainTable: public IMainTable {
    public:
    static std::string version();
    static std::string tableName();
    public:
    explicit PoolMainTable( coral::ISchema& dbSchema );
    virtual ~PoolMainTable();
    void setParameter( const std::string& paramName, const std::string& paramValue );
    bool getParameters( std::map<std::string,std::string>& destination );
    std::string schemaVersion();
    public:
    std::string name();
    bool exists();
    void create();
    void drop();
  };

  
  class PoolSequenceTable : public ISequenceTable{
    public:
    static std::string tableName();
    static std::string sequenceNameColumn();
    static std::string sequenceValueColumn();    
    public:
    explicit PoolSequenceTable( coral::ISchema& dbSchema );
    virtual ~PoolSequenceTable();
    void init( PoolDbCache& dbCache );
    bool add( const std::string& sequenceName );
    bool getLastId( const std::string& sequenceName, int& lastId );
    void sinchronize( const std::string& sequenceName, int lastValue );
    void erase( const std::string& sequenceName );
    public:
    std::string name();
    bool exists();
    void create();
    void drop();    
    private:
    PoolDbCache* m_dbCache;
  };

  class PoolMappingVersionTable: public IDatabaseTable {
    public:
    static std::string tableName();
    static std::string mappingVersionColumn();
    static std::string containerNameColumn();
    public:
    explicit PoolMappingVersionTable( coral::ISchema& dbSchema  );
    virtual ~PoolMappingVersionTable();
    public:
    std::string name();
    bool exists();
    void create();
    void drop();
  };
  
  class PoolMappingElementTable: public IDatabaseTable {
    public:
    static std::string tableName();
    static std::string mappingVersionColumn();
    static std::string elementIdColumn();
    static std::string elementTypeColumn();
    static std::string scopeNameColumn();
    static std::string variableNameColumn();
    static std::string variableParIndexColumn();
    static std::string variableTypeColumn();
    static std::string tableNameColumn();
    static std::string columnNameColumn();
    public:
    explicit PoolMappingElementTable( coral::ISchema& dbSchema  );
    virtual ~PoolMappingElementTable();
    public:
    std::string name();
    bool exists();
    void create();
    void drop();
  };
  
  class PoolContainerHeaderTable: public IContainerHeaderTable {
    public:
    static std::string tableName();
    static std::string containerIdColumn();
    static std::string containerNameColumn();
    static std::string containerTypeColumn();
    static std::string tableNameColumn();
    static std::string classNameColumn();
    static std::string baseMappingVersionColumn();
    static std::string numberOfWrittenObjectsColumn();
    static std::string numberOfDeletedObjectsColumn();
    static std::string homogeneousContainerType();
    public:
    explicit PoolContainerHeaderTable( coral::ISchema& dbSchema );
    virtual ~PoolContainerHeaderTable();
    void init( PoolDbCache& dbCache );
    bool getContainerData( std::map<std::string, ContainerHeaderData>& destination );
    void addContainer( int id, const std::string& containerName, const std::string& className );
    void removeContainer( int id );
    bool lockContainer( int id, ContainerHeaderData& destination );
    void incrementNumberOfObjects( int id  );
    void decrementNumberOfObjects( int id  );
    void updateNumberOfObjects( const std::map<int,unsigned int>& numberOfObjectsForContainerIds );
    public:
    std::string name();
    bool exists();
    void create();
    void drop();
    private:
    PoolDbCache* m_dbCache;
  };

  class PoolClassVersionTable: public IDatabaseTable {
    public:
    static std::string tableName();
    static std::string classVersionColumn();
    static std::string containerNameColumn();
    static std::string mappingVersionColumn();
    public:
    explicit PoolClassVersionTable( coral::ISchema& dbSchema  );
    virtual ~PoolClassVersionTable();
    public:
    std::string name();
    bool exists();
    void create();
    void drop();
  };

  class PoolMappingSchema: public IMappingSchema {
    public:
    static std::string emptyScope();
    public:
    explicit PoolMappingSchema( coral::ISchema& dbSchema );
    virtual ~PoolMappingSchema();
    void init( PoolDbCache& dbCache );
    public:
    bool getVersionList( std::set<std::string>& destination );
    bool getMapping( const std::string& version, MappingRawData& destination );
    void storeMapping( const MappingRawData& mapping );
    void removeMapping( const std::string& version );
    bool getContainerTableMap( std::map<std::string, int>& destination );
    bool getMappingVersionListForContainer( int containerId, std::set<std::string>& destination, bool onlyDependency=false );
    bool getMappingVersionListForTable( const std::string& tableName, std::set<std::string>& destination );
    bool getDependentClassesInContainerMapping( int containerId, std::set<std::string>& destination );
    bool getClassVersionListForMappingVersion( const std::string& mappingVersion, std::set<std::string>& destination );
    bool getClassVersionListForContainer( int containerId, std::map<std::string,std::string>& versionMap );
    bool selectMappingVersion( const std::string& classId, int containerId, std::string& destination );
    bool containerForMappingVersion( const std::string& mappingVersion, int& destination );
    void insertClassVersion( const std::string& className, const std::string& classVersion, const std::string& classId,
                             int dependencyIndex, int containerId, const std::string& mappingVersion );
    void setMappingVersion( const std::string& classId, int containerId, const std::string& mappingVersion );
    private:
    coral::ISchema& m_schema;
    PoolDbCache* m_dbCache;
  };

  class CondMetadataTable : public INamingServiceTable {
    public:
    static std::string tableName();
    static std::string objectNameColumn();
    static std::string tokenColumn();
    static std::string timetypeColumn();
   public: 
    CondMetadataTable( coral::ISchema& dbSchema, PoolDbCache& dbCache );
    virtual ~CondMetadataTable();
    void setObjectName( const std::string& name, int contId, int itemId );
    bool eraseObjectName( const std::string& name );
    bool eraseAllNames();
    bool getObjectByName( const std::string& name, std::pair<int,int>& destination );
    bool getNamesForObject( int contId, int itemId, std::vector<std::string>& destination );
    bool getNamesForContainer( int contId, std::vector<std::string>& destination );
    bool getAllNames( std::vector<std::string>& destination );
   public:
    std::string name();
    bool exists();
    void create();
    void drop();    
  private:
    PoolDbCache& m_dbCache;
  };
  
  class PoolDatabaseSchema: public IDatabaseSchema {
    public:
    static bool existsMainTable( coral::ISchema& dbSchema );
    public:
    explicit PoolDatabaseSchema( coral::ISchema& dbSchema );
    virtual ~PoolDatabaseSchema();

    bool exists();
    void create( const std::string& userSchemaVersion );
    void drop();
    void setAccessPermission( const std::string& principal, bool forWrite );

    IMainTable& mainTable();
    ISequenceTable& sequenceTable();
    IDatabaseTable& mappingVersionTable();
    IDatabaseTable& mappingElementTable();
    IContainerHeaderTable& containerHeaderTable();
    IDatabaseTable& classVersionTable();
    IMappingSchema& mappingSchema();
    INamingServiceTable& namingServiceTable();
    bool testDropPermission(){
      return false;
    }

    private:
    coral::ISchema& m_schema;
    PoolDbCache m_dbCache;
    PoolMainTable m_mainTable;
    PoolSequenceTable m_sequenceTable;
    PoolMappingVersionTable m_mappingVersionTable;
    PoolMappingElementTable m_mappingElementTable;
    PoolContainerHeaderTable m_containerHeaderTable;
    PoolClassVersionTable m_classVersionTable;
    PoolMappingSchema m_mappingSchema;
    CondMetadataTable m_metadataTable;
  };  
  
}

#endif
  
    
