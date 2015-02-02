enum nodeColor { red, black };

struct TreeNode {
   nodeColor color;
   int data;
   TreeNode* leftChild;
   TreeNode* rightChild;
   TreeNode* parent;

   TreeNode() {color = red;}
};

