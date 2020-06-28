#ifndef DG_BBLOCK_BASE_H_
#define DG_BBLOCK_BASE_H_

#include <vector>
#include <list>

namespace dg {

class ElemId {
    static unsigned idcnt;
    unsigned id;
public:
    ElemId() : id(++idcnt) {}
    unsigned getID() const { return id; }
};

template <typename ElemT>
class CFGElement : public ElemId {
    using EdgesT = std::vector<ElemT *>;

protected:
    EdgesT _successors;
    EdgesT _predecessors;

public:
    auto succ_begin() -> decltype(_successors.begin()) { return _successors.begin(); }
    auto succ_end() -> decltype(_successors.begin())  { return _successors.end(); }
    auto pred_begin() -> decltype(_predecessors.begin()) { return _predecessors.begin(); }
    auto pred_end() -> decltype(_predecessors.begin()) { return _predecessors.end(); }
    auto succ_begin() const -> decltype(_successors.begin()) { return _successors.begin(); }
    auto succ_end() const -> decltype(_successors.begin())  { return _successors.end(); }
    auto pred_begin() const -> decltype(_predecessors.begin()) { return _predecessors.begin(); }
    auto pred_end() const -> decltype(_predecessors.begin()) { return _predecessors.end(); }

    const EdgesT successors() const { return _successors; }
    const EdgesT predecessors() const { return _predecessors; }

    bool hasSuccessors() const { return !_successors.empty(); }
    bool hasPredecessors() const { return !_predecessors.empty(); }

    void addSuccessor(ElemT *s) {
        for (auto *succ : _successors) {
            if (succ == s)
                return;
        }

        _successors.push_back(s);

        for (auto *pred : s->_predecessors) {
            if (pred == this)
                return;
        }
        s->_predecessors.push_back(static_cast<ElemT*>(this));
    }

    ElemT *getSinglePredecessor() {
        return _predecessors.size() == 1 ? _predecessors.back() : nullptr;
    }

    ElemT *getSingleSuccessor() {
        return _successors.size() == 1 ? _successors.back() : nullptr;
    }
};

template <typename ElemT, typename NodeT>
class BBlockBase : public CFGElement<ElemT> {
public:
    using NodesT = std::list<NodeT *>;

    void append(NodeT *n) { _nodes.push_back(n); n->setBBlock(static_cast<ElemT*>(this)); }
    void prepend(NodeT *n) { _nodes.push_front(n); n->setBBlock(static_cast<ElemT*>(this)); }

    void insertBefore(NodeT *n, NodeT *before) {
        assert(!_nodes.empty());

        auto it = _nodes.begin();
        while (it != _nodes.end()) {
            if (*it == before)
                break;
            ++it;
        }
        assert(it != _nodes.end() && "Did not find 'before' node");

        _nodes.insert(it, n);
        n->setBBlock(static_cast<ElemT*>(this));
    }

    const NodesT& getNodes() const { return _nodes; }
    NodesT& getNodes() { return _nodes; }
    // FIXME: rename to first/front(), last/back()
    NodeT *getFirst() { return _nodes.empty() ? nullptr : _nodes.front(); }
    NodeT *getLast() { return _nodes.empty() ? nullptr : _nodes.back(); }
    const NodeT *getFirst() const { return _nodes.empty() ? nullptr : _nodes.front(); }
    const NodeT *getLast() const { return _nodes.empty() ? nullptr : _nodes.back(); }

    bool empty() const { return _nodes.empty(); }
    size_t size() const { return _nodes.size(); }

private:
    NodesT _nodes;
};

}

#endif
