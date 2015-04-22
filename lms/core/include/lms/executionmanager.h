#ifndef LMS_EXECUTION_MANAGER_H
#define LMS_EXECUTION_MANAGER_H

#include <string>
#include <deque>
#include <map>
#include <vector>
#include <memory>

#include "lms/module.h"
#include "lms/loader.h"
#include "lms/datamanager.h"
#include "lms/executionmanager.h"
#include "lms/logger.h"

namespace lms {

class DataManager;
class Loader;

class ExecutionManager {
public:

    ExecutionManager(logging::Logger &rootLogger);
    ~ExecutionManager();

    /**cycle modues */
    void loop();

    /**
     * Searches the programm for all availabe modules and adds them to the availabe list
     */
    void loadAvailabelModules();
    /**
     * Enable module with the given name, add it to the cycle-queue
     */
    void enableModule(const std::string &name, logging::LogLevel minLogLevel
                      = logging::SMALLEST_LEVEL);
    /**
     * @brief Disable module with the given name, remove it from the
     * cycle-queue.
     *
     * @param name name of the module that should be disabled
     * @return true if disabling was successful, false otherwise
     */
    bool disableModule(const std::string &name);


    void invalidate();
    /**
     * @brief validate if invalidate was called before, it will check if all selected modules are initialised
     * Sorts the modules in the cycle-list
     */
    void validate();
private:
    logging::Logger &rootLogger;
    logging::ChildLogger logger;

    int maxThreads;
    bool valid;

    Loader loader;
    DataManager dataManager;
    Messaging messaging;

    /**
     * @brief enabledModules contains all loaded Modules
     */
    std::vector<Module*> enabledModules;
    /**
     * @brief cycleListT the cycleListType
     */
    typedef std::vector<std::vector<Module*>> cycleListType;
    cycleListType cycleList;
    /**
     * @brief available contains all Modules which can be loaded
     */
    Loader::moduleList available;
    /**
     * @brief sort call this method for sorting the cycleList
     * As this method isn't called often I won't care about performance but readability. If you have to call this method often you might have to improve it.
     */
    void sort();
    /**
     * @brief sortByDataChannel Sorts enableModules into cycleList. Call this method before sortByPriority().
     * As this method isn't called often I won't care about performance but readability. If you have to call this method often you might have to improve it.
     */
    void sortByDataChannel();
    /**
     * @brief sortByDataChannel Sorts loadedModules into cycleList
     * As this method isn't called often I won't care about performance but readability. If you have to call this method often you might have to improve it.
     */
    void sortByPriority();
};

}  // namespace lms

#endif /* LMS_EXECUTION_MANAGER_H */
