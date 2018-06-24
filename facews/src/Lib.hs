{-# LANGUAGE DataKinds       #-}
{-# LANGUAGE TemplateHaskell #-}
{-# LANGUAGE TypeOperators   #-}
module Lib
    ( startApp
    , app
    ) where

import Control.Concurrent
import Control.Concurrent.STM.TVar
import Control.Monad.IO.Class
import Control.Monad.STM
import Control.Monad.Trans.Reader
import Data.Aeson
import Data.Aeson.TH
import Network.Wai
import Network.Wai.Handler.Warp
import Servant

type API = "autorizado" :> Get '[JSON] Bool
    :<|> "autorizar" :> Get '[JSON] ()
    :<|> "revocar" :> Get '[JSON] ()

startApp :: IO ()
startApp = do
    initial  <- atomically $ newTVar False
    run 8080 $ app $ State initial

data State = State { state :: TVar Bool }

nt :: State -> ReaderT State Handler a -> Handler a
nt s x = runReaderT x s

app :: State -> Application
app s = serve api $ hoistServer api (nt s) server

api :: Proxy API
api = Proxy

server :: ServerT API (ReaderT State Handler)
server = state :<|> authorize :<|> revoke
    where
        state = do
            State auth <- ask
            liftIO $ atomically $ readTVar auth

        authorize = do
            State auth <- ask
            liftIO $ atomically $ writeTVar auth True
            return ()
        revoke = do
            State auth <- ask
            liftIO $ atomically $ writeTVar auth False
            return ()
