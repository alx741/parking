{-# LANGUAGE DataKinds         #-}
{-# LANGUAGE DeriveGeneric     #-}
{-# LANGUAGE LambdaCase        #-}
{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE TypeFamilies      #-}
{-# LANGUAGE TypeOperators     #-}

module App where

import Control.Monad.IO.Class (liftIO)
import Control.Monad.Logger   (runStderrLoggingT)
-- import           Database.Persist.Sqlite ( ConnectionPool, createSqlitePool
--                                          , runSqlPool, runSqlPersistMPool
--                                          , runMigration, selectFirst, (==.)
--                                          , insert, entityVal)
import Data.String.Conversions  (cs)
import Data.Text                (Text)
import Database.Persist.MySQL   (ConnectInfo(..), ConnectionPool, createMySQLPool,
                                 entityVal, insert, runMigration,
                                 runSqlPersistMPool, runSqlPool, selectFirst,
                                 (==.), connectPath, defaultConnectInfo)
import Network.Wai.Handler.Warp as Warp

import Servant

import Api
import Models


-- connStr = "host=localhost dbname=webserv user=alx password=verde port=3306"

connInfo :: ConnectInfo
connInfo = ConnectInfo
    { connectHost = "localhost"
    , connectPort = 3306
    , connectUser = "alx"
    , connectPassword = "verde"
    , connectDatabase = "webserv"
    , connectOptions = []
    , connectPath = connectPath defaultConnectInfo
    , connectSSL = Nothing
    }

server :: ConnectionPool -> Server Api
server pool =
  userAddH :<|> userGetH
  where
    userAddH newUser = liftIO $ userAdd newUser
    userGetH name    = liftIO $ userGet name

    userAdd :: User -> IO (Maybe (Key User))
    userAdd newUser = flip runSqlPersistMPool pool $ do
      exists <- selectFirst [UserName ==. (userName newUser)] []
      case exists of
        Nothing -> Just <$> insert newUser
        Just _  -> return Nothing

    userGet :: Text -> IO (Maybe User)
    userGet name = flip runSqlPersistMPool pool $ do
      mUser <- selectFirst [UserName ==. name] []
      return $ entityVal <$> mUser

app :: ConnectionPool -> Application
app pool = serve api $ server pool

mkApp :: IO Application
mkApp = do
  pool <- runStderrLoggingT $ do
    -- createMySQLPool (cs sqliteFile) 5
    createMySQLPool connInfo 5

  runSqlPool (runMigration migrateAll) pool
  return $ app pool

run :: IO ()
run =
  Warp.run 3000 =<< mkApp
