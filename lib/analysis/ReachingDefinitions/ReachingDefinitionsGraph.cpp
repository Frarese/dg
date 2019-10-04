#include <set>
#include <vector>

#include "dg/analysis/ReachingDefinitions/ReachingDefinitions.h"
#include "dg/analysis/BBlocksBuilder.h"

namespace dg {
namespace analysis {
namespace rd {

void ReachingDefinitionsGraph::buildBBlocks(bool dce) {
    assert(getRoot() && "No root node");
    DBG(dda, "Building basic blocks");

    BBlocksBuilder<RDBBlock> builder;
    _bblocks = std::move(builder.buildAndGetBlocks(getRoot()));

    assert(getRoot()->getBBlock() && "Root node has no BBlock");

    // should we eliminate dead code?
    // The dead code are the nodes that have no basic block assigned
    // (follows from the DFS nature of the block builder algorithm)
    if (!dce)
        return;

    for (auto& nd : _nodes) {
        if (nd->getBBlock() == nullptr) {
            nd->isolate();
            nd.reset();
        }
    }
}

void ReachingDefinitionsGraph::removeUselessNodes() {
}


} // namespace rd
} // namespace analysis
} // namespace dg