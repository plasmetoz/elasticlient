/**
 * \file
 * Elasticsearch Scroll API implementation.
 */

#pragma once

#include "elasticlient/scroll.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <json/json.h>
#include "elasticlient/client.h"


namespace elasticlient {


class Scroll::Implementation {
     /// Struct for store Elastic scroll parameters
     struct ScrollParams {
        /// Id of current Elasticsearch scroll
        std::string scrollId;
        /// Elastichsearch query body
        std::string searchBody;
        /// Elasticsearch document type
        std::string docType;
        /// Elasticsearch index to be operated on
        std::string indexName;

        ScrollParams() : scrollId(), searchBody(), docType(), indexName()
        {}


        /// Clear all variable members
        void clear();
    };

    /// Client holder
    std::shared_ptr<Client> client;
    /// Number of results returned in one Elasticsearch call.
    std::size_t scrollSize;
    /// Tells Elasticsearch to keep the search context open for another timeout
    std::string scrollTimeout;
    /// Elastic current scroll parameters
    ScrollParams scrollParameters;

    friend class Scroll;
    friend class ScrollByScan;
  public:
    Implementation(std::shared_ptr<Client> elasticClient,
                   std::size_t scrollSize,
                   const std::string &scrollTimeout)
      : client(std::move(elasticClient)), scrollSize(scrollSize), scrollTimeout(scrollTimeout),
        scrollParameters()
    {
        if (!client) {
            throw std::runtime_error("Valid Client instance is required.");
        }
    }

    /// Is scroll initialized (Was init() called?)
    bool isInitialized() const {
        return (!scrollParameters.searchBody.empty() && !scrollParameters.docType.empty());
    }

    /// Is scroll started (Was scrollId generated by Elasticsearch?)
    bool isScrollStarted() const {
        return !scrollParameters.scrollId.empty();
    }

    /// Run request on Client
    bool run(const std::string &commonUrlPart, const std::string &body, Json::Value &parsedResult);

    ///Parse Elasticsearch HTTP \p result into \p parsedResult
    bool parseResult(const std::string &result, Json::Value &parsedResult);
};


}  // namespace elasticlient
