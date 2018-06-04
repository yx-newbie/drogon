/**
 *
 *  @file
 *  @author An Tao
 *  @section LICENSE
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  @section DESCRIPTION
 *
 */

#pragma once

#include <drogon/HttpApiBinder.h>
#include <trantor/utils/NonCopyable.h>
#include <drogon/DrObject.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/PostFilter.h>
#include <drogon/GetFilter.h>
#include <drogon/PutFilter.h>
#include <drogon/DeleteFilter.h>
#include <drogon/version.h>
#include <drogon/NotFound.h>
#include <memory>
#include <string>
#include <functional>
#include <vector>
namespace drogon
{
    //the drogon banner
    const char banner[]=       "     _                             \n"
                               "  __| |_ __ ___   __ _  ___  _ __  \n"
                               " / _` | '__/ _ \\ / _` |/ _ \\| '_ \\ \n"
                               "| (_| | | | (_) | (_| | (_) | | | |\n"
                               " \\__,_|_|  \\___/ \\__, |\\___/|_| |_|\n"
                               "                 |___/             \n";
    inline std::string getVersion()
    {
        return VERSION;
    }
    inline std::string getGitCommit()
    {
        return VERSION_MD5;
    }
    class HttpAppFramework:public trantor::NonCopyable
    {
    public:
        static HttpAppFramework &instance();
        virtual void addListener(const std::string &ip,uint16_t port)=0;
        virtual void run()=0;
        virtual ~HttpAppFramework();
        virtual void registerHttpSimpleController(const std::string &pathName,
                                                  const std::string &crtlName,
                                                  const std::vector<std::string> &filters=std::vector<std::string>())=0;
        virtual void registerHttpApiController(const std::string &pathName,
                                               const std::string &parameterPattern,
                                               const HttpApiBinderBasePtr &binder,
                                               const std::vector<std::string> &filters=std::vector<std::string>())=0;
        template <typename FUNCTION>
        static void registerHttpApiMethod(const std::string &pathName,
                                   const std::string &parameterPattern,
                                   FUNCTION && function,
                                   const std::vector<std::string> &filters=std::vector<std::string>())
        {
            static_assert(!std::is_bind_expression<FUNCTION>::value);
            HttpApiBinderBasePtr binder=std::make_shared<
                    HttpApiBinder<FUNCTION>
            >(std::forward<FUNCTION>(function));
            instance().registerHttpApiController(pathName,parameterPattern,binder,filters);
        }
//        template <typename FUNCTION>
//        static void registerHttpApiBind(const std::string &pathName,
//                                          const std::string &parameterPattern,
//                                          const std::function<FUNCTION> & function,
//                                          const std::vector<std::string> &filters=std::vector<std::string>())
//        {
//            HttpApiBinderBasePtr binder=std::make_shared<
//                    HttpApiBinder<decltype(function)>
//            >(std::forward<decltype(function)>(function));
//            instance().registerHttpApiController(pathName,parameterPattern,binder,filters);
//        }
        virtual void enableSession(const size_t timeout=0)=0;
        virtual void disableSession()=0;
    };
}