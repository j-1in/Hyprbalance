#include "include/DwindleBalancer.hpp"

#include <hyprland/src/Compositor.hpp>

#define private public
#include <hyprland/src/managers/LayoutManager.hpp>
#undef private

static void printNodeInfo(SDwindleNodeData* node) {
    if (!node)
        return;
    // std::cout << "Node: " << node->pWindow.lock()->m_title;
    std::cout << "node: " << node->box.pos().x << ", " << node->box.pos().y
              << " size: " << node->box.size().x << "x" << node->box.size().y
    << " ratio: " << node->splitRatio
    << std::endl;
}

static void traverse(SDwindleNodeData* node) {
    if (!node) {
        std::cout << "Node is nullptr" << std::endl;
        return;
    }

    // visit the node
    printNodeInfo(node);

    // traverse left and right children
    traverse(node->children.at(0));
    traverse(node->children.at(1));
}

// FIXME: crash when on floating window
void CDwindleBalancer::balanceColOfWindows() {
    const auto activeWindow = g_pCompositor->m_lastWindow.lock();
    const auto* activeNode = dLayout->getNodeFromWindow(activeWindow);
    const auto RNODE = dLayout->getMasterNodeOnWorkspace(activeNode->workspaceID);
    traverse(RNODE);
    std::cout << "\n";

    // find the (leaf nodes) in the same column as the active window
    std::set<SDwindleNodeData*> nodesInColumn;
    int windowCount = 0;
    for (auto& n : dLayout->m_dwindleNodesData) {
        if ((n.workspaceID == activeWindow->workspaceID() && n.valid)) {
            // check if leaf node and in the same column
            if (!n.isNode
                && n.box.pos().x == activeNode->box.pos().x
                && n.box.size().x == activeNode->box.size().x) {
                auto* insertNode = &n;

                // check only the parent node for the same column and only leaf children
                if (n.pParent
                    && (n.pParent->box.pos().x == n.box.pos().x)
                    && (n.pParent->box.size().x == n.box.size().x)
                    && (!n.pParent->children.at(0)->isNode && !n.pParent->children.at(1)->isNode)) {
                    insertNode = n.pParent;
                }

                auto ret = nodesInColumn.insert(insertNode);

                if (ret.second != false) {
                    // add 2 window counts if the node is a parent containing 2 vertically split leaf children
                    if (insertNode->isNode) {
                        if (insertNode->children.at(0)->box.pos().x
                            == insertNode->children.at(1)->box.pos().x) {
                            windowCount += 2; // new parent node insertion
                        }
                    } else {
                        ++windowCount;
                    }
                }
            } else if (n.isNode && n.box.pos().x == activeNode->box.pos().x
                       && n.box.size().x == activeNode->box.size().x
                       && (n.children.at(0)->box.pos().x != activeNode->box.pos().x
                           || n.children.at(0)->box.size().x != activeNode->box.size().x
                           || n.children.at(1)->box.pos().x != activeNode->box.pos().x
                           || n.children.at(1)->box.size().x != activeNode->box.size().x)) {
                // if the node is a parent and all its children are not in line with the column
                auto ret = nodesInColumn.insert(&n);
                if (ret.second != false) {
                    ++windowCount;
                }
            }
        }
    }

    // find the longest continuous theoretical box containing the activeNode
    double top = activeNode->box.pos().y;
    while (true) {
        bool found = false;
        for (auto& n : nodesInColumn) {
            double nodeBottom = n->box.pos().y + n->box.size().y;
            // if the bottom of the node touches the top of the active node then it is continuous
            if (nodeBottom == top) {
                top = n->box.pos().y;
                found = true;
                break; // continue to find the next node
            }
        }
        if (!found) {
            break; // no more nodes found
        }
    }

    double bottom = activeNode->box.pos().y + activeNode->box.size().y;
    while (true) {
        bool found = false;
        for (auto& n : nodesInColumn) {
            double nodeTop = n->box.pos().y;
            // if the top of the node touches the bottom of the active node then it is continuous
            if (nodeTop == bottom) {
                bottom = n->box.pos().y + n->box.size().y;
                found = true;
                break; // continue to find the next node
            }
        }
        if (!found) {
            break; // no more nodes found
        }
    }

    // remove all other nodes that are not in the continuous block
    // for (auto it = nodesInColumn.begin(); it != nodesInColumn.end();) {
    //     if ((*it)->box.pos().y < top || (*it)->box.pos().y + (*it)->box.size().y > bottom) {
    //         windowCount -= (*it)->isNode ? 2 : 1; // remove the node count FIXME
    //         it = nodesInColumn.erase(it); // remove the node
    //     } else {
    //         ++it; // move to the next node
    //     }
    // }

    // now the only problem is that some nodes might have 2 windows that horizontally split

    std::cout << "Found " << nodesInColumn.size() << " nodes in the same column as the active window." << std::endl;
    for (auto& n : nodesInColumn) {
        std::cout << "Node: " << n->box.pos().x << ", " << n->box.pos().y
                  << " size: " << n->box.size().x << "x" << n->box.size().y
                  << " ratio: " << n->splitRatio
                  << std::endl;
    }
    std::cout << "Found " << windowCount << " windows in the same column as the active window." << std::endl;
    std::cout << "----------------------------------------------------------\n";


    for (auto& n : nodesInColumn) {
        // get the neighbor node if it exists
        std::pair<int, SDwindleNodeData*> neighborNodeInfo {0, nullptr};
        if (n->isNode) {
            n->splitRatio = 1.f; // reset split ratio
        }
        if (n->pParent) {
            if (n->pParent->children.at(0) == n) {
                neighborNodeInfo = {0, n->pParent->children.at(1)};
            } else {
                neighborNodeInfo = {1, n->pParent->children.at(0)};
            }
        }

        if (neighborNodeInfo.second && !neighborNodeInfo.second->isNode
            && nodesInColumn.count(neighborNodeInfo.second) > 0) {
            if (n->isNode) {
                // if n is a parent node, and its neighboring child is a leaf node
                if (neighborNodeInfo.first) {
                    // if the neighbor node is on the right, split ratio = 2/3
                    n->pParent->splitRatio = 0.66f;
                } else {
                    // if the neighbor node is on the left, split ratio = 1/3
                    n->pParent->splitRatio = 0.33f;
                }
            } else {
                // if n is a leaf node, set split ratio to 1
                n->pParent->splitRatio = 1.f;
            }
        }

        // if
    }


    dLayout->recalculateMonitor(activeWindow->monitorID());
}

void CDwindleBalancer::balanceRowOfWindows() {
    // find all
    const auto activeWindow = g_pCompositor->m_lastWindow.lock();
    for (auto& n : dLayout->m_dwindleNodesData) {
        if (n.workspaceID == activeWindow->workspaceID() && n.valid) {
            n.splitRatio = 1.f; // reset split ratio
        }
    }

    dLayout->recalculateMonitor(activeWindow->monitorID());
}

void CDwindleBalancer::balanceAllWindows() {
    const auto activeWindow = g_pCompositor->m_lastWindow.lock();
    for (auto& n : dLayout->m_dwindleNodesData) {
        if (n.workspaceID == activeWindow->workspaceID() && n.valid) {
            n.splitRatio = 1.f; // reset split ratio
        }
    }

    dLayout->recalculateMonitor(activeWindow->monitorID());
}
