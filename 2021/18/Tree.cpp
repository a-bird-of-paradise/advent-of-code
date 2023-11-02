#include "Tree.hpp"
#include <iostream>

namespace aoc {

    void LeafNode::print(unsigned long long depth, const unsigned long long maxdepth)
    {
        for(unsigned long long i = 0; i < depth; i++) std::cout << "|"; 
        std::cout << "[" << this << "]" << ", value: " << this->get_value() << "\n"; 
    }

    void BranchNode::print(unsigned long long depth, const unsigned long long maxdepth)
    {
        for(unsigned long long i = 0; i < depth; i++) std::cout << "|"; 
        std::cout << "[" << this << "]" << "\n"; 
        if (depth < maxdepth)
        {
            if(this->left_.get() != nullptr) 
                this->left_->print(depth+1,maxdepth);
            if(this->right_.get() != nullptr) 
                this->right_->print(depth+1,maxdepth);
        }
    }

    LeafNode* LeafNode::NextLeaf()
    {
        BranchNode *parent;
        Node *current;

        // algo: seek up the tree until I am a left child
        // then switch to right branch
        // and return leftmost descendant

        current = this;

        while(1) {
            parent = current->get_parent();
            if(parent == nullptr) return nullptr;
            if(current == parent->get_left().get()) break; 
            current = parent;
        }
        current = current->get_parent()->get_right().get();
        while(!current->IsLeaf()) current = dynamic_cast<BranchNode*>(current)->get_left().get();

        return dynamic_cast<LeafNode*>(current);
    }

    LeafNode* LeafNode::PrevLeaf()
    {
        BranchNode *parent;
        Node *current;

        // algo: mirror image of NextLeaf

        current = this;

        while(1) {
            parent = current->get_parent();
            if(parent == nullptr) return nullptr;
            if(current == parent->get_right().get()) break; 
            current = parent;
        }
        current = current->get_parent()->get_left().get();
        while(!current->IsLeaf()) current = dynamic_cast<BranchNode*>(current)->get_right().get();

        return dynamic_cast<LeafNode*>(current);
    }

    int LeafNode::get_depth()
    {
        BranchNode* parent = this->get_parent();
        int answer = 0;
        while(parent != nullptr) {
            answer++;
            parent = parent->get_parent();
        }
        return answer;
    }

    std::unique_ptr<BranchNode> BranchNode::clone() const
    {
        //four cases
        if(this->get_left()->IsLeaf() && this->get_right()->IsLeaf())
        {
            // two numbers, nice and easy
            return std::make_unique<BranchNode>(
                std::make_unique<LeafNode>(dynamic_cast<LeafNode*>(this->get_left().get())->get_value()),
                std::make_unique<LeafNode>(dynamic_cast<LeafNode*>(this->get_right().get())->get_value())
            );
        }
        else if (this->get_left()->IsLeaf() && !this->get_right()->IsLeaf())
        {
            return std::make_unique<BranchNode>(
                std::make_unique<LeafNode>(dynamic_cast<LeafNode*>(this->get_left().get())->get_value()),
                dynamic_cast<BranchNode*>(this->get_right().get())->clone()
            );
        }
        else if (!this->get_left()->IsLeaf() && this->get_right()->IsLeaf())
        {
            return std::make_unique<BranchNode>(
                dynamic_cast<BranchNode*>(this->get_left().get())->clone(),
                std::make_unique<LeafNode>(dynamic_cast<LeafNode*>(this->get_right().get())->get_value())
            );
        }
        else 
        {
            return std::make_unique<BranchNode>(
                dynamic_cast<BranchNode*>(this->get_left().get())->clone(),
                dynamic_cast<BranchNode*>(this->get_right().get())->clone()
            );
        }
    }

    void reduce_tree(BranchNode *tree)
    {
        bool clean_run_through = false;

        while(!clean_run_through)
        {
            clean_run_through = true;
            // any explosions?
            for(LeafNode* i = tree->FirstLeaf(); i != nullptr; i = i->NextLeaf()) {
                if(i->get_depth() > 4) {

                    clean_run_through = false;

                    BranchNode *ToExplode, *ToExplodeParent;
                    ToExplode = i->get_parent();
                    ToExplodeParent = ToExplode->get_parent();

                    bool IsLeft = ToExplode == ToExplodeParent->get_left().get();

                    LeafNode *prev, *next;
                    prev = dynamic_cast<LeafNode*>(ToExplode->get_left().get())->PrevLeaf();
                    next = dynamic_cast<LeafNode*>(ToExplode->get_right().get())->NextLeaf();

                    int leftval, rightval;
                    leftval = dynamic_cast<LeafNode*>(ToExplode->get_left().get())->get_value();
                    rightval= dynamic_cast<LeafNode*>(ToExplode->get_right().get())->get_value();

                    if(prev != nullptr) prev->set_value(prev->get_value() + leftval);
                    if(next != nullptr) next->set_value(next->get_value() + rightval);

                    if(IsLeft)
                        ToExplodeParent->set_left(std::make_unique<LeafNode>(0));
                    else
                        ToExplodeParent->set_right(std::make_unique<LeafNode>(0));

                    //std::cout << "EXLODED\n";
                    //tree->print();
                    //std::cout << '\n';    
                    break;
                }
            }
            if(!clean_run_through) continue; // jump here after first explosion and loop 
            // phew now any to split? 
            for(LeafNode* i = tree->FirstLeaf(); i != nullptr; i = i->NextLeaf()) {
                if(i->get_value() >= 10) {
                    clean_run_through = false;

                    int left_val = i->get_value()/2; // int division
                    int right_val = i->get_value() - left_val;

                    BranchNode *ToSplitParent = i->get_parent();
                    bool IsLeft = i == ToSplitParent->get_left().get();

                    if(IsLeft) 
                        ToSplitParent->set_left(std::make_unique<BranchNode>(
                            std::make_unique<LeafNode>(left_val), 
                            std::make_unique<LeafNode>(right_val)
                        ));
                    else
                        ToSplitParent->set_right(std::make_unique<BranchNode>(
                            std::make_unique<LeafNode>(left_val), 
                            std::make_unique<LeafNode>(right_val)
                        ));

                    //std::cout << "SPLIT\n";
                    //tree->print();
                    //std::cout << '\n';    
                    break;
                }
            }
        // phew. any splits mean we loop again. but now reduced! 
        }
    }
}