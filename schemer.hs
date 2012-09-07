module Main where

import System.Environment
import Text.ParserCombinators.Parsec hiding (spaces)
import Control.Monad

symbol :: Parser Char
symbol = oneOf "!$%&|*+-/:<=>?@^_~"

spaces :: Parser ()
spaces = skipMany1 space

data LispVal = Atom String  
             | Number Integer   
             | String String  
             | Bool Bool			 
             | List [LispVal]   
             | DottedList [LispVal] LispVal  
               
-- String
parseString :: Parser LispVal 
parseString = do
  char '"'
  x <- many (noneOf "\"")
  char '"'
  return $ String x

-- atom
parseAtom :: Parser LispVal
parseAtom = do
  first <- letter <|> symbol
  rest <- many (letter <|> digit <|> symbol)
  let atom = [first] ++ rest
  return $ case atom of 
    "#t" -> Bool True
    "#f" -> Bool False
    otherwise -> Atom atom

-- number
parseNumber :: Parser LispVal
parseNumber = liftM (Number . read) $ many1 digit
	
parseNumber2 :: Parser LispVal
parseNumber2 = do
	x <- many1 digit
	return $ (Number . read) x

parseNumber3 :: Parser LispVal
parseNumber3 = many1 digit >>= \p -> return $ (Number . read) p

-- list of LispVal
parseList :: Parser LispVal
parseList = liftM List $ sepBy parseExpr spaces

-- (a . b)
parseDottedList :: Parser LispVal 
parseDottedList = do
	head <- endBy parseExpr spaces 
	tail <- char '.' >> spaces >> parseExpr
	return $ DottedList head tail

-- quote
parseQuoted :: Parser LispVal 
parseQuoted = do
	char '\''
	x <- parseExpr
	return $ List [Atom "quote", x]
	
-- expression
parseExpr :: Parser LispVal
parseExpr = parseAtom 
		<|> parseString 
		<|> parseNumber3
		<|> parseQuoted 
		<|> do 
				char '('
				x <- (try parseList) <|> parseDottedList 
				char ')'
				return x
				

-- 
readExpr1 :: String -> String
readExpr1 input = case parse parseExpr "list" input of
	Left err -> "No match: " ++ show err
	Right val -> showVal val

readExpr :: String -> LispVal 
readExpr input = case parse parseExpr "scheme" input of 
	Left err -> String $ "No match: " ++ show err
	Right val -> val
-- 
showVal :: LispVal -> String

showVal (String contents) = "(String) -- \"" ++ contents ++ "\""
showVal (Atom name) = "(Atom) -- " ++ name
showVal (Number contents) = "(Number) -- " ++ show contents
showVal (Bool True) = "(Bool) -- #t"
showVal (Bool False) = "(Bool) -- #f"

showVal (List contents) = "(" ++ unwordsList contents ++ ")"
showVal (DottedList head tail) = "(" ++ unwordsList head ++ " . " ++ showVal tail ++ ")"

-- helper function for showVal
unwordsList :: [LispVal] -> String
unwordsList = unwords . map showVal

-- 
instance Show LispVal where 
	show = showVal

-- evaluate (4.2)
eval :: LispVal -> LispVal
eval val@(String _) = val
eval val@(Number _) = val
eval val@(Bool _) = val
eval (List [Atom "quote", val]) = val 
-- function application
eval (List (Atom func : args)) = apply func $ map eval args

apply :: String -> [LispVal] -> LispVal
apply func args = maybe (Bool False) ($ args) $ lookup func primitives

primitives :: [(String , [LispVal] -> LispVal)]
primitives = [
	("+", numericBinop (+)),
	("-", numericBinop (-)),
	("*", numericBinop (*)),
	("/", numericBinop div),
	("mod", numericBinop mod),
	("quotient", numericBinop quot),
	("remainder", numericBinop rem)
	]

--
numericBinop :: (Integer -> Integer -> Integer) -> [LispVal] -> LispVal
numericBinop op params = Number $ foldl1 op $ map unpackNum params

-- 
unpackNum :: LispVal -> Integer 
unpackNum (Number n) = n
unpackNum (String n) = let parsed = reads n in if null parsed then 0 else fst $ parsed !! 0
unpackNum (List [n]) = unpackNum n
unpackNum _ = 0

-- interactive evaluate
ie :: String -> String 
ie = show . eval . readExpr

main :: IO ()
main = getArgs >>= putStrLn . show . eval . readExpr . (!! 0)