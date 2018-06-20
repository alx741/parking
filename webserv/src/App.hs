{-# LANGUAGE DataKinds         #-}
{-# LANGUAGE DeriveGeneric     #-}
{-# LANGUAGE LambdaCase        #-}
{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE TypeFamilies      #-}
{-# LANGUAGE TypeOperators     #-}

module App where

import Control.Monad.IO.Class (liftIO)
import Control.Monad.Logger   (runStderrLoggingT)
import Data.String.Conversions  (cs)
import Data.Text                (Text)
import Database.Persist.Types
import Database.Persist.MySQL   (ConnectInfo(..), ConnectionPool, createMySQLPool,
                                 entityVal, insert, runMigration,
                                 runSqlPersistMPool, runSqlPool, selectFirst, selectList, get,
                                 (==.), connectPath, defaultConnectInfo)
import Network.Wai.Handler.Warp as Warp

import Servant

import Api
import Models


connInfo :: ConnectInfo
connInfo = ConnectInfo
    -- { connectHost = "localhost"
    { connectHost = "parking1.mysql.database.azure.com"
    , connectPort = 3306
    , connectUser = "alx@parking1"
    , connectPassword = "Parking741"
    , connectDatabase = "Parking"
    , connectOptions = []
    , connectPath = connectPath defaultConnectInfo
    , connectSSL = Nothing
    }

server :: ConnectionPool -> Server Api
server pool =
  usuarioGetH :<|> edificioGetH :<|> bloqueGetH
  where
    usuarioGetH email    = liftIO $ userGet email
    edificioGetH id      = liftIO $ edificioGet id
    bloqueGetH id        = liftIO $ bloqueGet id

    userGet :: Text -> IO (Maybe Usuario)
    userGet email = flip runSqlPersistMPool pool $ do
      mUser <- selectFirst [UsuarioEmail ==. email] []
      return $ entityVal <$> mUser

    edificioGet :: EdificioId -> IO (Maybe Edificio)
    edificioGet id = flip runSqlPersistMPool pool $ get id

    bloqueGet :: EdificioId -> IO [Bloque]
    bloqueGet id = flip runSqlPersistMPool pool $ do
        bloques <- selectList [BloqueEdificioId ==. id] []
        return $ entityVal <$> bloques


app :: ConnectionPool -> Application
app pool = serve api $ server pool

mkApp :: IO Application
mkApp = do
  pool <- runStderrLoggingT $ do
    createMySQLPool connInfo 5

  runSqlPool (runMigration migrateAll) pool
  return $ app pool

run :: IO ()
run =
  Warp.run 3000 =<< mkApp
