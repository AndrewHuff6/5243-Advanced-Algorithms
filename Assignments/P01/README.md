In my bst.cpp file, I handled the three cases mentioned in the readme file. Those were:
  - Deleting a leaf node
  - Deleting a node with one child
  - Deleting a node with two children
Before I started to code, I also drew example pictures just to jog my memory on how BSTs work overall.
Then, I looked at the entire BST class to determine it's general structure. Lastly, I took the liberty
to draw out some examples as well as watch a couple youtube videos to help vizualize operations.
I decided to go with the inorder predecessor because it made the most general sense to me when I wrote
it out on paper. In my mind, I didn't want to bother messing with the right side of the BST (for 
whatever reason). 
In all honesty, this assignment was pretty straight forward. Some of the only difficulties I've had with
this one were the inline prototype (which I confirmed was syntactically correct with ChatGPT) and making
sure that my logic for the third case was correct. I was a bit freaked out when big words like "predecessor"
and "successor" were thrown around in the readme file, but ultimately it was easy to just create a new node
and setting it to the subroot's data. I think also our in-class examples of stacks and queues helped me out 
in the sense of using -> rather than a dot operator. 
