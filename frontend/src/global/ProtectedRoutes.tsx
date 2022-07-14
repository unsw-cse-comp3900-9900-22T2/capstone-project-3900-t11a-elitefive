import React from 'react'
import {
  Navigate,
  Outlet,
  useNavigate
} from 'react-router-dom';

import { useAuth } from './GlobalAuth';


type Props = {
}

export default function ProtectedRoutes({}: Props) {
  const { isLogged } = useAuth();
  const navigate = useNavigate();

  if(!isLogged()) {
    navigate('/login');
  }
  return <Outlet/>
}