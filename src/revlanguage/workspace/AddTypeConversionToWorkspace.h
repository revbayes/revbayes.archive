//#ifndef AddTypeConversionToWorkspace_H
//#define AddTypeConversionToWorkspace_H
//
//#include <string>
//
//#include "Func__"
//#include "WorkspaceVector.h"
//#include "Workspace.h"
//
//namespace RevLanguage {
//    
//    template <typename objType, int d>
//    class AddTypeConversionToWorkspace {
//        
//    public:
//        //!< Add the given object and a vector version of it.
//        static bool                 addTypeToWorkspace( Workspace &w, objType *o ) {
//            w.addType( o );
//            
//            w.addFunction("v", new Func_workspaceVector<objType>() );
//            AddVectorizedWorkspaceType<WorkspaceVector<objType>, d-1>::addTypeToWorkspace( w, new WorkspaceVector<objType>() );
//            return true;
//        }
//    };
//    
//    template <typename objType>
//    class AddTypeConversionToWorkspace<objType,0> {
//        
//    public:
//        //!< Add the given object.
//        static bool                 addTypeToWorkspace( Workspace &w, objType *o ) {
//            w.addType( o );
//            return true;
//        }
//    };
//    
//}
//
//
//
//#endif
//
