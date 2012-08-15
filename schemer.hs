module Main where

import System.Environment
import Text.ParserCombinators.Parsec hiding (spaces)

symbol :: Parser Char
symbol = oneOf "!$%&|*+-/:<=>?@^_~"

spaces :: Parser ()
spaces = skipMany1 space

readExpr :: String -> String
readExpr input = case parse (spaces >> symbol) "list" input of
  Left err -> "No match: " ++ show err
  Right val -> "Found value: " ++ show val

data LispVal = Atom String  
             | List [String]   
             | DottedList [LispVal] LispVal  
             | Number Integer   
             | String String  
             | Bool Bool
               
-- String
parseString :: Parser LispVal 
parseString = do
  char '"'
  x <- many (noneOf "\"")
  char '"'
  return $ String x

parseAtom :: Parser LispVal
parseAtom = do
  first <- letter <|> symbol
  rest <- many (letter <|> digit <|> symbol)
  let atom = [first] ++ rest
  return $ case atom of 
    "#t" -> Bool True
    "#f" -> Bool False
    otherwise -> Atom atom

main :: IO ()
main = do
  args <- getArgs
  putStrLn (readExpr (args !! 0))