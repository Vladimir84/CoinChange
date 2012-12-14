import sys
import argparse
import re

def combinations(denominations,target_sum):
    print denominations
    table=[]
    zero_record=[0 for i in range(len(denominations))]
    table.append([zero_record])
    # initilalize table with empty lists
    for i in range(target_sum): table.append([])
        
    for coin_id in range(len(denominations)):
        print "coin value is ", denominations[coin_id]
        for i in range(denominations[coin_id],target_sum+1):
            entry=table[i-denominations[coin_id]]
            if entry:
               for combination in entry:
                   com=list(combination)
                   com[coin_id]+=1
                   table[i].append(com)
    return table[target_sum]        
    
   
def isFloat(string):
    try:
        num=float(string)
        return (True,num)
    except ValueError:
        return (False,0)


def print_combinations(currency,combinations):
    print currency
    print combinations
    
    
def validNumber(number):
    if number==0:
        print "coin value is zero"
        return False
    if number<0:
        print "negative coin value"
        return False
    if number<1:
        print "coin value is smaller then target sum"
        return False
    return True
    
    
def processInput(denominations):
    tokens=re.split('[,]',denominations)
    coins={}
    if not len(tokens)%2 == 0:
        print "Incorrect argument, perhaps coin or its value is missing?"
        return False
    else:
        for i in range(0,len(tokens),2):
            (isnumber,number)=isFloat(tokens[i])
            if isnumber:
                print ("token %d is expected to be a word, but appeares to be a number [%d]" %
                (i,number))
                return False
            (isnumber,number)=isFloat(tokens[i+1])
            if not isnumber:
                print ("token %d is expected to be a number, it is a word [%d] instead" %
                 (i,tokens[i+1]))
                return False
            if not validNumber(number): return False
            coins[tokens[i]]=number
    return coins
    
def convert2denoms(coins):
    target_sum=int(max(coins.values()))
    values=[]
    for k in coins: values.append(int(target_sum//coins[k]))
    return (target_sum,values)
    
    
def main():
    parser=argparse.ArgumentParser()
    parser.add_argument("denominations",help="comma delimited list which specifies the name of each denomination along with the number required of that denomination to reach our target sum", nargs="?", default="Quater,4,Dime,10,Nickel,20,Penny,100")
    args=parser.parse_args()

    coins= processInput(args.denominations)
    print coins.items()
    if not coins:
        sys.exit(1)
    (target_sum,denominations)=convert2denoms(coins)
    
    all_combinations=combinations(denominations,target_sum)
    print len(all_combinations)
    
    print_combinations(coins.keys(),all_combinations)
    sys.exit(0)
if __name__ == '__main__':
  main()