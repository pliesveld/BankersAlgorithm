Banker's Algorithm
==================

The banker's algorithm is a deadlock detection and avoidance algorithm for resource allocation of multiple instances of each resource type.  The system is initialized with the total resources available of each type.  Before a process enters the system, it must declare the maximum number of each resource type it can allocate.  The system will not grant resources if it leaves the system in an unsafe state.



Example
-------

        BankerSystem sys{{'A','B','C','D','E','F','G'},
                         {  1,  2,  3,  4,  5,  6,  7}};

        sys.UpdateResourcesAvailable();

        sys.AddProcess(Process{{0,2,0,0,0,2,7}});
        sys.AddProcess(Process{{0,0,0,4,5,2,0}});
        sys.AddProcess(Process{{1,0,3,0,0,2,0}});
        sys.AddProcess(Process{{1,0,0,0,0,1,0}});

        sys.UpdateResourcesAvailable();

        canGrantRequest(sys,0);
        canGrantRequest(sys,1);
        canGrantRequest(sys,2);

        assert(isSafeState(sys));

        assert(canGrantRequest(sys,3) == false);
