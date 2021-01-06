import System.Random

main = do
    password <- randomRIO (1, 100) :: IO Int
    choice <- prompt "Do you want to block or unblock a website? [b/u] "
    case choice of
        "b" -> putStrLn "not nice"
        "u" -> unblock password

prompt :: String -> IO String
prompt text = do
    putStr text
    getLine

unblock :: Int -> IO ()
unblock password = do
    guess <- prompt "Password: "
    if (read guess :: Int) == password
        then putStrLn "nice"
        else unblock password
