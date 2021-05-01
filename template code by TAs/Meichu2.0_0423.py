import numpy as np

# Black go first
BLACK = 0
WHITE = 1
SELF_BARRIER_COST = -3
OPPONENT_BARRIER_COST = 3
MAX_PRICE = 140
S = [' ', '◆', '◇', '●', '○', '■', '□']
# 0: empty, 1: black flag, 2: white flag, 3: black pieces,
# 4: white pieces, 5: black barriers, 6: white barriers

class Meichu():
    def __init__(self):
        self.game_over = False
        self.checkerboard = np.zeros([9,9],dtype=int)
        self.n_pieces = [5, 5]
        self.n_barriers = [12, 12]
        self.budget = [150, 150]
        self.color = -1

        ### board initialization ###
        self.checkerboard[0,4] = 2  # white flag
        self.checkerboard[-1,4] = 1 # black flag

        loc = [[0,2], [0,6], [1,3], [1,5], [2,4]]
        for l in loc:
            self.checkerboard[l[0], l[1]] = 4  # white pieces
            self.checkerboard[8-l[0], l[1]] = 3 # black pieces
            
        # barriers
        loc = [[0,3], [0,5], [1,1], [1,4], [1,7], [2,1], [2,7], [3,2], [3,3], [3,4], [3,5], [3,6]]
        for l in loc:
            self.checkerboard[l[0], l[1]] = 6  # white barriers
            self.checkerboard[8-l[0], l[1]] = 5 # black barriers
        
    def bid(self):
        ####################################################
        ##### You can modify the setting by yourself ! #####
        ####################################################
        # color -> BLACK:0  WHITE:1
        # price -> an non-negative <int> value
        color_choice = BLACK
        price = 5
        return color_choice, price
    
    def make_decision(self, who):
        #######################################################
        ##### This is the main part you need to implement #####
        #######################################################
        # return format : [begin_x, begin_y, end_x, end_y]
        if who == self.color:
            print("My turn :")
            # Use AI to make decision !
            # input() is only for testing !
            move = [int(x) for x in input("Enter the move : ").split()]
            return move
        else:
            return None
    
    def start(self):
        # bid for black or white
        # please set "bid" function by yourself
        # bid function will return two <int> values
        color_choice, price = self.bid() 
        price = min(int(price),MAX_PRICE)
        print('My bid: ', color_choice, price)
        opponent_color_choice = int(input('Please input opponent_color_choice :'))
        opponent_price = int(input('Please input opponent_price :'))
        opponent_price = min(opponent_price,MAX_PRICE)
        assert (color_choice==0 or color_choice==1) and (opponent_color_choice==0 or opponent_color_choice==1)
        if color_choice != opponent_color_choice:
            self.color = color_choice
        elif price > opponent_price:
            self.color = color_choice
            self.budget[self.color] -= max(0, opponent_price)+1
        elif price < opponent_price:
            self.color = (1-opponent_color_choice)
            self.budget[1-self.color] -= max(0, price)+1
        else: 
            # Tie -> set player by TA
            print('Tie !')
            self.color = int(input('Please set my player number :'))
            if(self.color == color_choice):
                self.budget[self.color] -= max(0, opponent_price)+1
            else:
                self.budget[1-self.color] -= max(0, price)+1
            
        assert self.color==0 or self.color==1
        print('My color is: {}'.format('BLACK' if self.color == BLACK else 'WHITE'))     
        
        step = 0
        
        while not self.game_over:
            self.show_board()
            if step%2==BLACK: # black's turn
                if self.budget[BLACK] <= 1:
                    print('No budget to move! Only to pass!')
                    step += 1
                    continue
                if self.color==BLACK:
                    print('My color is: {}'.format('BLACK' if self.color == BLACK else 'WHITE'))
                    move = self.make_decision(BLACK)
                    print('My move :',move)
                    self.make_move(BLACK,move[0],move[1],move[2],move[3])
                else:
                    print('Opponent color is: {}'.format('WHITE' if self.color == BLACK else 'BLACK'))
                    print("Opponent turn :")
                    move = [int(x) for x in input("Enter the move : ").split()]
                    self.make_move(BLACK,move[0],move[1],move[2],move[3])
                step += 1
            else: # white's turn
                if self.budget[WHITE] <= 1:
                    print('No budget to move! Only to pass!')
                    step += 1
                    continue
                if self.color==WHITE:
                    print('My color is: {}'.format('BLACK' if self.color == BLACK else 'WHITE'))
                    move = self.make_decision(WHITE)
                    print('My move :',move)
                    self.make_move(WHITE,move[0],move[1],move[2],move[3])
                else:
                    print('Opponent color is: {}'.format('WHITE' if self.color == BLACK else 'BLACK'))
                    print("Opponent turn :")
                    move = [int(x) for x in input("Enter the move : ").split()]
                    self.make_move(WHITE,move[0],move[1],move[2],move[3])
                step += 1
        self.terminate()

    def get_pieces(self, color):
        ##############################################################
        ##### You can remove this function if you don't need it. #####
        ##############################################################
        b = np.zeros([9,9], dtype=int)
        if color == BLACK:
            x, y = np.where(self.checkerboard == 4)
        else:
            x, y = np.where(self.checkerboard == 3)
        for i, j in zip(x, y):
            b[i, j] = 1
        return b

    def get_barriers(self, color):
        ##############################################################
        ##### You can remove this function if you don't need it. #####
        ##############################################################
        b = np.zeros([9,9], dtype=int)
        if color == BLACK:
            x, y = np.where(self.checkerboard == 6)
        else:
            x, y = np.where(self.checkerboard == 5)
        for i, j in zip(x, y):
            b[i, j] = 1
        return b

    def make_move(self, who, begin_x, begin_y, end_x, end_y):
        
        # if who == 0 -> balck
        # if who == 1 -> white
        
        piece = who+3
        
        assert self.checkerboard[begin_x, begin_y] == piece
        assert begin_x >= 0 and begin_x < 9 and end_x >= 0 and end_x < 9 \
          and begin_y >= 0 and begin_y < 9 and end_y >= 0 and end_y < 9
        assert begin_x == end_x or begin_y == end_y
        assert begin_x != end_x or begin_y != end_y
        check_sum = ((begin_x==end_x)*sum(self.checkerboard[begin_x,min(begin_y,end_y)+1:max(begin_y,end_y)]) \
                  + (begin_y==end_y)*sum(self.checkerboard[min(begin_x,end_x)+1:max(begin_x,end_x),begin_y]))
        assert check_sum == 0
        
        distance = abs(begin_x-end_x) + abs(begin_y-end_y);
        
        if who == BLACK:
            self.budget[BLACK] -= (1 + distance)
            assert self.budget[BLACK] >= 0
            if self.checkerboard[end_x, end_y] == 0:
                pass
            elif self.checkerboard[end_x, end_y] == 2:
                self.game_over = True
            elif self.checkerboard[end_x, end_y] == 6:
                print('Break white barrier!')
                self.n_barriers[WHITE] -= 1
                self.budget[BLACK] -= OPPONENT_BARRIER_COST
            elif self.checkerboard[end_x, end_y] == 4:
                print('Take white piece!')
                self.n_pieces[WHITE] -= 1
                if self.n_pieces[WHITE] == 0:
                    self.game_over = True
            elif self.checkerboard[end_x, end_y] == 5:
                print('Break black barrier!')
                self.n_barriers[BLACK] -= 1
                self.budget[BLACK] -= SELF_BARRIER_COST
            else:
                raise Exception('Do not move your piece to an occupied place!')
            assert self.budget[BLACK] >= 0
        else:
            self.budget[WHITE] -= (1 + distance)
            assert self.budget[WHITE] >= 0
            if self.checkerboard[end_x, end_y] == 0:
                pass
            elif self.checkerboard[end_x, end_y] == 1:
                self.game_over = True
            elif self.checkerboard[end_x, end_y] == 5:
                print('Break black barrier!')
                self.n_barriers[BLACK] -= 1
                self.budget[WHITE] -= OPPONENT_BARRIER_COST
            elif self.checkerboard[end_x, end_y] == 3:
                print('Take black piece!')
                self.n_pieces[BLACK] -= 1
                if self.n_pieces[BLACK] == 0:
                    self.game_over = True
            elif self.checkerboard[end_x, end_y] == 6:
                print('Break white barrier!')
                self.n_barriers[WHITE] -= 1
                self.budget[WHITE] -= SELF_BARRIER_COST
            else:
                raise Exception('Do not move your piece to an occupied place!')
            assert self.budget[WHITE] >= 0
        
        # Move
        self.checkerboard[begin_x, begin_y] = 0
        self.checkerboard[end_x, end_y] = piece
        print('Player {} moved piece from ({},{}) to ({},{})' \
              .format('BLACK' if who==BLACK else 'WHITE',begin_x,begin_y,end_x,end_y))
        if self.budget[BLACK] <= 1 and self.budget[WHITE] <= 1:
            self.game_over = True

    def terminate(self):
        # number == 0
        if self.checkerboard[0, 4] == 3:
            print('BLACK wins!')
        elif self.checkerboard[-1, 4] == 4:
            print('WHITE wins!')
        elif self.n_pieces[BLACK] > self.n_pieces[WHITE]:
            print('BLACK wins!')
        elif self.n_pieces[BLACK] < self.n_pieces[WHITE]:
            print('WHITE wins!')
        elif self.n_barriers[BLACK] > self.n_barriers[WHITE]:
            print('BLACK wins!')
        elif self.n_barriers[BLACK] < self.n_barriers[WHITE]:
            print('WHITE wins!')
        else:
            print('Draw!')
            
    def show_board(self):
        print('――――――――――――――――――')
        print('Budget: ')
        print('        Black: ', self.budget[BLACK])
        print('        White: ', self.budget[WHITE])
        print('n_pieces: ')
        print('        Black: ', self.n_pieces[BLACK])
        print('        White: ', self.n_pieces[WHITE])
        print('n_barriers: ')
        print('        Black: ', self.n_barriers[BLACK])
        print('        White: ', self.n_barriers[WHITE])
        print()
        print(' y 0 1 2 3 4 5 6 7 8 ')
        print('x  ― ― ― ― ― ― ― ― ― ')
        i = 0
        for line in self.checkerboard:
            print(i,'|{}|{}|{}|{}|{}|{}|{}|{}|{}|'.format(S[line[0]],S[line[1]],S[line[2]], \
                        S[line[3]],S[line[4]],S[line[5]],S[line[6]],S[line[7]],S[line[8]]))
            print('   ― ― ― ― ― ― ― ― ― ')
            i += 1
        print('――――――――――――――――――')


if __name__ == '__main__':
    game = Meichu()
    game.start()
    input("Please press the Enter key to exit ...")

