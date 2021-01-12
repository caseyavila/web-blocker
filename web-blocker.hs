import System.IO
import System.Random

main = do
    unblock
    password <- randomRIO (1, 100) :: IO Int
    choice <- prompt "Do you want to block or unblock a website? [b/u] "
    case choice of
        "b" -> putStrLn "Blocking password..."
        "u" -> guessing password

prompt :: String -> IO String
prompt text = do
    putStr text
    hFlush stdout
    getLine

guessing :: Int -> IO ()
guessing password = do
    guess <- prompt "Password: "
    if (read guess :: Int) == password
        then putStrLn "Correct password"
        else guessing password

unblock :: IO ()
unblock = do
    handle <- openFile "hosts" ReadMode
    contents <- hGetContents handle 
    print $ map findDomain (unCommented contents)
    hClose handle


unCommented :: String -> [String]
unCommented input = stringKiller $ lines input 

stringKiller :: [String] -> [String]
stringKiller [] = []
stringKiller (x : xs)
    | x == [] = stringKiller xs
    | head x == '#' = stringKiller xs
    | otherwise = x : stringKiller xs

findDomain :: String -> String
findDomain line = head $ words line
