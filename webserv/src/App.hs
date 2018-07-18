{-# LANGUAGE DataKinds         #-}
{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE TypeFamilies      #-}
{-# LANGUAGE TypeOperators     #-}

module App where

import Control.Monad            (forM_)
import Control.Monad.IO.Class   (liftIO)
import Control.Monad.Logger     (runStderrLoggingT)
import Data.String.Conversions  (cs)
import Data.Text                (Text)
import Data.Time.Clock
import Data.Time.Clock.POSIX
import Database.Persist.MySQL   (ConnectInfo (..), ConnectionPool, connectPath,
                                 createMySQLPool, defaultConnectInfo, entityVal,
                                 get, insert, runMigration, runSqlPersistMPool,
                                 runSqlPool, selectFirst, selectList, update,
                                 updateWhere, (=.), (==.))
import Database.Persist.Types
import Network.Wai.Handler.Warp as Warp

import Servant

import Api
import Models


connInfo :: ConnectInfo
connInfo = ConnectInfo
    -- { connectHost = "parking1.mysql.database.azure.com"
    { connectHost = "localhost"
    , connectPort = 3306
    , connectUser = "alx"
    , connectPassword = "verde"
    , connectDatabase = "Parking"
    , connectOptions = []
    , connectPath = connectPath defaultConnectInfo
    , connectSSL = Nothing
    }

server :: ConnectionPool -> Server Api
server pool =
       usuarioGetH
  :<|> edificioGetH
  :<|> bloqueGetH
  :<|> puestoGetH
  :<|> puestoIsReservedH
  :<|> puestoReserveH
  :<|> puestoWipeReserveH
  where
    usuarioGetH email        = liftIO $ userGet email
    edificioGetH id          = liftIO $ edificioGet id
    bloqueGetH id            = liftIO $ bloqueGet id
    puestoGetH id            = liftIO $ puestoGet id
    puestoIsReservedH id pid = liftIO $ puestoIsReserved id pid
    puestoReserveH id pid    = liftIO $ puestoReserve id pid
    puestoWipeReserveH       = liftIO $ puestoWipeReserve

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

    puestoGet :: BloqueId -> IO [Puesto]
    puestoGet id = flip runSqlPersistMPool pool $ do
        puestos <- selectList [PuestoBloqueId ==. id] []
        return $ entityVal <$> puestos

    puestoIsReserved :: BloqueId -> Int -> IO Bool
    puestoIsReserved id pid = flip runSqlPersistMPool pool $ do
        puesto <- selectFirst [PuestoBloqueId ==. id, PuestoPuesto ==. pid] []
        case puesto of
            Just p  -> return $ puestoReservado $ entityVal p
            Nothing -> return False

    puestoReserve :: BloqueId -> Int -> IO ()
    puestoReserve id pid = flip runSqlPersistMPool pool $ do
        time <- liftIO getCurrentTime
        updateWhere [PuestoBloqueId ==. id, PuestoPuesto ==. pid] [PuestoReservado =. True, PuestoReservadoAt =. time]
        return ()

    puestoWipeReserve :: IO ()
    puestoWipeReserve = flip runSqlPersistMPool pool $ do
        curTime <- liftIO getCurrentTime
        spots <- selectList [PuestoReservado ==. True] []

        let reservedOverDue = filter
                (\(Entity _ a) -> isOverDue curTime $ puestoReservadoAt a)
                spots

        forM_ reservedOverDue (\a -> update (entityKey a) [PuestoReservado =. False, PuestoReservadoAt =. curTime])
        return ()


isOverDue :: UTCTime -> UTCTime -> Bool
isOverDue currentTime placementTime = placementTime < expireTime
    where expireTime = posixSecondsToUTCTime $ utcTimeToPOSIXSeconds currentTime - 5

app :: ConnectionPool -> Application
app pool = serve api $ server pool

mkApp :: IO Application
mkApp = do
  pool <- runStderrLoggingT $ createMySQLPool connInfo 5

  runSqlPool (runMigration migrateAll) pool
  return $ app pool

run :: IO ()
run =
  Warp.run 3000 =<< mkApp
