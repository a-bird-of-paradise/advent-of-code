#pragma once
#include <memory>

namespace aoc
{

    class BranchNode;
    class LeafNode;

    class Node
    {
    public:
        Node() {parent_ = nullptr;}
        virtual bool IsLeaf() = 0;
        virtual void print(unsigned long long depth = 0, const unsigned long long maxdepth = 999) = 0;
        virtual ~Node() {}
        virtual int magnitude() = 0;
        virtual BranchNode* get_parent() {return parent_;}
        virtual void set_parent(BranchNode* parent) { parent_ = parent;}
        virtual LeafNode* FirstLeaf() = 0;
        virtual LeafNode* LastLeaf() = 0;
    protected:
        BranchNode* parent_;
    };

    class BranchNode : public Node
    {
    public:
        BranchNode(std::unique_ptr<Node> left, std::unique_ptr<Node> right)
            {   set_left(std::move(left)); set_right(std::move(right));   }

        virtual ~BranchNode() {}

        void set_left(std::unique_ptr<Node> left) { left_ = std::move(left); left_->set_parent(this); }
        void set_right(std::unique_ptr<Node> right) { right_ = std::move(right); right_->set_parent(this); }

                std::unique_ptr<Node>& get_left(void)           { return left_; }
                std::unique_ptr<Node>& get_right(void)          { return right_; }
        const   std::unique_ptr<Node>& get_left(void)   const   { return left_; }
        const   std::unique_ptr<Node>& get_right(void)  const   { return right_; }

        virtual bool IsLeaf() { return false; }
        virtual void print(unsigned long long depth = 0, const unsigned long long maxdepth = 999);
        virtual int magnitude() { return 3*left_->magnitude() + 2*right_->magnitude(); }
        virtual LeafNode* FirstLeaf() {return left_->FirstLeaf();}
        virtual LeafNode* LastLeaf() {return right_->LastLeaf();}

        virtual std::unique_ptr<BranchNode> clone() const;

    private:
        std::unique_ptr<Node> left_, right_;
    };

    class LeafNode : public Node
    {
    public:
        LeafNode(int value) : value_(value) {}
        virtual ~LeafNode() {}

        virtual bool IsLeaf() { return true; }

                int get_value(void)         { return value_;    }
        const   int get_value(void) const   { return value_;    }
        
        void set_value(const int value) { value_ = value;   }
        virtual void print(unsigned long long depth = 0, const unsigned long long maxdepth = 999);

        virtual int magnitude() { return value_; }
        virtual LeafNode* FirstLeaf() {return this;}
        virtual LeafNode* LastLeaf() {return this;}
        virtual LeafNode* NextLeaf();
        virtual LeafNode* PrevLeaf();
        virtual int get_depth();

    private:
        int value_;
    };

    void reduce_tree(BranchNode* tree);
}