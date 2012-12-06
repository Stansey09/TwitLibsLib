#include "TwitLibsLib.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "includes/twitcurl.h"

namespace TwitLibsLib
{


	void TwitLibs::replaceWords(std::string &theString)
	{
		int index = -1;
		int wordIndex;
		index = theString.find("<noun>");
		while (index != -1)
		{
			wordIndex = (std::rand() % vNouns.size());
			theString.replace(index, 6, vNouns[wordIndex].c_str());
			index = -1;
			index = theString.find("<noun>");
		}

		index = -1;
		index = theString.find("<verb>");
		while (index != -1)
		{
			wordIndex = (std::rand() % vVerbs.size());
			theString.replace(index, 6, vVerbs[wordIndex].c_str());
			index = -1;
			index = theString.find("<verb>");
		}

		index = -1;
		index = theString.find("<adjective>");
		while (index != -1)
		{
			wordIndex = (std::rand() % vAdjectives.size());
			theString.replace(index, 11, vAdjectives[wordIndex].c_str());
			index = -1;
			index = theString.find("<adjective>");
		}

		if (theString.length() > 140)
		{
			theString = theString.substr(0, 140);
		}

		printf("Posting this status: %s ",  theString.c_str());

		std::cin.get();
	
	}

	void TwitLibs::loadWords()
	{
		std::ifstream inFile;
		std::string readString;
		inFile.open("nouns.txt");
		std::getline(inFile, readString);
		while (!inFile.eof())
		{
			vNouns.push_back(readString);
			std::getline(inFile, readString);
		}

		inFile.close();
		

		inFile.open("verbs.txt");
		std::getline(inFile, readString);
		while (!inFile.eof())
		{
			vVerbs.push_back(readString);
			std::getline(inFile, readString);
		}

		inFile.close();

		inFile.open("adjectives.txt");
		std::getline(inFile, readString);
		while (!inFile.eof())
		{
			vAdjectives.push_back(readString);
			std::getline(inFile, readString);
		}

		inFile.close();
	}

	void TwitLibs::doTwitLibs()
	{
		loadWords();
		std::string userName( "" );
		std::string passWord( "" );

		printf( "Enter twitter username: ");
		getline(std::cin, userName);
		printf( "Enter twitter password: ");
		getline(std::cin, passWord);

		twitCurl twitterObj;
		std::string tmpStr;
		std::string replyMsg;
		char tmpBuf[1024];

		/* Set twitter username and password */
		twitterObj.setTwitterUsername( userName );
		twitterObj.setTwitterPassword( passWord );


		/* OAuth flow begins */
		/* Step 0: Set OAuth related params. These are got by registering your app at twitter.com */
		twitterObj.getOAuth().setConsumerKey( std::string( "M0732hNbsNWFAt5EI2z8bA" ) );
		twitterObj.getOAuth().setConsumerSecret( std::string( "iI6jbNN3rn7tZmOsMvSiWXcYo9e36HzWwpZdEJvh7bs" ) );

		/* Step 1: Check if we alredy have OAuth access token from a previous run */
		std::string myOAuthAccessTokenKey("85586080-mEQUEMw1tiId6XaC3xW6k0U4C3uQMRHGYWWaP6Qw");
		std::string myOAuthAccessTokenSecret("ghtJ0vSe5Z8LYAMurSXjvN2rfEkOxuxIgYMU6lAF138");
		std::ifstream oAuthTokenKeyIn;
		std::ifstream oAuthTokenSecretIn;
		/*
		oAuthTokenKeyIn.open( "twitterClient_token_key.txt" );
		oAuthTokenSecretIn.open( "twitterClient_token_secret.txt" );

		memset( tmpBuf, 0, 1024 );
		oAuthTokenKeyIn >> tmpBuf;
		myOAuthAccessTokenKey = tmpBuf;

		memset( tmpBuf, 0, 1024 );
		oAuthTokenSecretIn >> tmpBuf;
		myOAuthAccessTokenSecret = tmpBuf;

		oAuthTokenKeyIn.close();
		oAuthTokenSecretIn.close();
		*/

		if( myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size() )
		{
			/* If we already have these keys, then no need to go through auth again */
			printf( "\nUsing:\nKey: %s\nSecret: %s\n\n", myOAuthAccessTokenKey.c_str(), myOAuthAccessTokenSecret.c_str() );

			twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
			twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
		}
		else
		{
			/* Step 2: Get request token key and secret */
			std::string authUrl;
			twitterObj.oAuthRequestToken( authUrl );

			/* Step 3: Get PIN  */
			memset( tmpBuf, 0, 1024 );
			printf( "\nDo you want to visit twitter.com for PIN (0 for no; 1 for yes): " );
			gets( tmpBuf );
			tmpStr = tmpBuf;
			if( std::string::npos != tmpStr.find( "1" ) )
			{
				/* Ask user to visit twitter.com auth page and get PIN */
				memset( tmpBuf, 0, 1024 );
				printf( "\nPlease visit this link in web browser and authorize this application:\n%s", authUrl.c_str() );
				printf( "\nEnter the PIN provided by twitter: " );
				gets( tmpBuf );
				tmpStr = tmpBuf;
				twitterObj.getOAuth().setOAuthPin( tmpStr );
			}
			else
			{
				/* Else, pass auth url to twitCurl and get it via twitCurl PIN handling */
				twitterObj.oAuthHandlePIN( authUrl );
			}
			/* Step 4: Exchange request token with access token */
			twitterObj.oAuthAccessToken();

			/* Step 5: Now, save this access token key and secret for future use without PIN */
			twitterObj.getOAuth().getOAuthTokenKey( myOAuthAccessTokenKey );
			twitterObj.getOAuth().getOAuthTokenSecret( myOAuthAccessTokenSecret );

			/* Step 6: Save these keys in a file or wherever */
			std::ofstream oAuthTokenKeyOut;
			std::ofstream oAuthTokenSecretOut;

			oAuthTokenKeyOut.open( "twitterClient_token_key.txt" );
			oAuthTokenSecretOut.open( "twitterClient_token_secret.txt" );

			oAuthTokenKeyOut.clear();
			oAuthTokenSecretOut.clear();

			oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
			oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

			oAuthTokenKeyOut.close();
			oAuthTokenSecretOut.close();
		}
		/* OAuth flow ends */

		/* Account credentials verification */
		if( twitterObj.accountVerifyCredGet() )
		{
			twitterObj.getLastWebResponse( replyMsg );
			printf( "\ntwitterClient:: twitCurl::accountVerifyCredGet web response:\n%s\n", replyMsg.c_str() );
		}
		else
		{
			twitterObj.getLastCurlError( replyMsg );
			printf( "\ntwitterClient:: twitCurl::accountVerifyCredGet error:\n%s\n", replyMsg.c_str() );
		}
	
		/* Post a new status message */
		memset( tmpBuf, 0, 1024 );
		printf( "\nEnter a new status message: " );
		gets( tmpBuf );
		tmpStr = tmpBuf;

		replaceWords(tmpStr);

		replyMsg = "";
	
		if( twitterObj.statusUpdate( tmpStr ) )
		{
			twitterObj.getLastWebResponse( replyMsg );
			//printf( "\ntwitterClient:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
		}
		else
		{
			twitterObj.getLastCurlError( replyMsg );
			//printf( "\ntwitterClient:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
		}
	
		std::cin.get();
	}

}